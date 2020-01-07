#include <iostream>

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

struct Guide {
   int x,y,d,e;
   Guide(int x,int y,int d,int e) : x(x),y(y),d(d),e(e){}
};

struct Case {
   int c,g,k,a;
   long b;
   std::vector<Guide> guides;
   
   Case() {
      std::cin >> c >> g >> b >> k >> a;
      guides.reserve(g);
      for(int i=0; i<g; i++) {
         int x,y,d,e;
	 std::cin >> x >> y >> d >> e;
	 guides.push_back(Guide(x,y,d,e));
      }
      int neRes = 0;
      long cost = 0;
      check(b,neRes,cost);

      if(cost <= b) {
         std::cout << neRes << std::endl;
	 return;
      }
      //std::cout << neRes << " " << cost << std::endl;
      
      int lower = 0;
      int upper = neRes;
      while(lower < upper-1) {
         int m = (lower + upper)/2;
	 check(m,neRes,cost);
	 //std::cout << m << " " << neRes << " " << cost << " " << b << std::endl;
	 //std::cout << lower << " " << upper << std::endl;
	 if(cost <= b) {
	    lower = m;
	 } else {
	    upper = m;
	 }
      }
      check(lower,neRes,cost);
      std::cout << neRes << std::endl;
   }

   void check(int ne, int &neRes, long &cost) {
      graph G(c+g+2);
      edge_adder adder(G);
      int src = c+g;
      int dst = src+1;
      
      auto c_map = boost::get(boost::edge_capacity, G);
      auto r_map = boost::get(boost::edge_reverse, G);
      auto rc_map = boost::get(boost::edge_residual_capacity, G);

      int gg = c;
      for(const Guide &ggg : guides) {
         adder.add_edge(ggg.x,gg,ggg.e,ggg.d);
         adder.add_edge(gg,ggg.y,ggg.e,0);
	 gg++;
      }
      adder.add_edge(src,k,ne,0);
      adder.add_edge(a,dst,ne,0);

      boost::successive_shortest_path_nonnegative_weights(G, src, dst);
      long cost_f = boost::find_flow_cost(G);
      long s_flow = 0;
      out_edge_it e, eend;
      for(boost::tie(e, eend) = boost::out_edges(boost::vertex(src,G), G); e != eend; ++e) {
          s_flow += c_map[*e] - rc_map[*e];     
      }
      neRes = s_flow;
      cost = cost_f;
   }
};

int main() {
   std::ios_base::sync_with_stdio(false);

   int t;
   std::cin >> t;
   for(int tt=0; tt<t; tt++) {
      Case c;
   }
}



