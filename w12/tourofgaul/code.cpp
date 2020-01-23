#include <iostream>
#include <map>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

// Custom edge adder class
class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
    //std::cout << "e: " << from << " " << to << " - " << capacity << " " << cost << std::endl;
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G); // new!
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;   // new assign cost
    w_map[rev_e] = -cost;   // new negative cost
  }
};

struct Edge {
   int u,v,d;
   Edge(int u, int v,int d) : u(u),v(v),d(d) {}
};
bool operator < (const Edge& e1, const Edge& e2) {
   return (e1.u < e2.u or (e1.u==e2.u and (e1.v < e2.v or (e1.v==e2.v and e1.d < e2.d))));
}

int main() {
   std::ios_base::sync_with_stdio(false);

   int t;
   std::cin >> t;
   for(int tt=0; tt<t; tt++) {
      int n,m;
      std::cin >> n >> m;
      
      std::map<Edge,int> edge;
      
      int id = 0;
      int src = id++;
      int dst = id++;
      std::vector<int> node(n);
      for(int i=0; i<n; i++) {node[i] = id++;}

      std::vector<int> c(n-1);
      int maxC = 0;
      for(int i=0; i<n-1; i++) {
         std::cin >> c[i];
	 maxC = std::max(maxC, c[i]);
      }

      std::vector<int> link(128*n);
      
      for(int i=0; i<m; i++) {
         int a,b,d;
	 std::cin >> a >> b >> d;
         
	 Edge ee(a,b,d);
	 const auto &it = edge.find(ee);
	 if(it == edge.end()) {edge[ee] = 0;}
	 edge[ee]++;

	 if(link[128*a + d-1] == 0) {
	    link[128*a + d-1] = id++;
	 }
      }

      int totalIn = maxC;
      graph G(id);
      edge_adder adder(G);


      adder.add_edge(src, node[0], c[0], 0);
      adder.add_edge(node[n-1], dst, c[n-2], 0);
      for(int i=0; i<n-1; i++) {
	 adder.add_edge(node[i], node[i+1], c[i], 0);
      }
      for(int i=1; i<n-1; i++) {
	 int diff = c[i] - c[i-1];
	 if(diff > 0) {
	    // add some flow:
            adder.add_edge(src, node[i+1], diff, 0);
            totalIn+=diff;
	 } else {
	    adder.add_edge(node[i], dst, -diff, 0);
	 }
      }
      
      for(int i=0; i<n; i++) {
         for(int d=0; d<128; d++) {
            int l=link[128*i + d];
	    if(l>0) {
	       adder.add_edge(node[i],l, maxC, -(d+1));
	    }
	 }
      }

      for(const auto &it : edge) {
         const Edge &ee = it.first;
	 int cap = it.second;

	 int l = link[128*ee.u + ee.d-1];
	 assert(l!=0);
	 //std::cout << "place: " << ee.u << " " << ee.v << " " << ee.d << std::endl;
	 //std::cout << " " << cap << std::endl;
	 adder.add_edge(l, node[ee.v], cap, 0);
      }
      
      int flow = boost::push_relabel_max_flow(G, src, dst);
      boost::cycle_canceling(G);
      int cost = boost::find_flow_cost(G);
      //std::cout << "f: " << flow << " c: " << cost << " exp: " << totalIn << std::endl;
      assert(flow == totalIn);
      std::cout << -cost << std::endl;
      //std::cout << "hello " << id << std::endl;
   }
}

