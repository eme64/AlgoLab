#include <iostream>
#include <vector>
#include <map>


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

struct Test {
   int n, s;
   std::vector<int> l;
   std::vector<std::map<int,int>> iandt2id;
   
   Test() {
      std::cin >> n >> s;
      l.resize(s);
      iandt2id.resize(s);
      int maxCap = 0;
      int maxT = 0;
      for(int i=0; i<s; i++){
         std::cin >> l[i];
	 maxCap+=l[i];
      }
      
      graph G(2+n*2);
      edge_adder adder(G);

      for(int i=0; i<n; i++) {
         int si,ti,di,ai,pi;
	 std::cin >> si >> ti >> di >> ai >> pi;
         int i1 = getId(si-1,di);
         int i2 = getId(ti-1,ai);
         int dt = ai-di;
	 adder.add_edge(i1, i2, 1, dt*100 - pi);
	 maxT = std::max(maxT, ai);
	 //std::cout << "r: " << i1 << " " << i2 << std::endl;
      }
      
      int expFlow = 0;
      for(int i=0; i<s; i++) {
         int ii = 0;
	 int lastid = -1;
	 int lastt = 0;
	 for(auto &it : iandt2id[i]) {
	    int t = it.first;
	    int id = it.second;
            
	    if(ii++==0) {
	       adder.add_edge(src, id, l[i], t*100);
	       expFlow+=l[i];
	       //std::cout << "inj: " << id << std::endl;
	    }
	    if(lastid >=0) {
	       adder.add_edge(lastid, id, maxCap, (t-lastt)*100);
	       //std::cout << "link: " << lastid << " " << id << std::endl;
	    }
	    lastt = t;
	    lastid = id;
	 }
	 if(lastid >=0) {
	    adder.add_edge(lastid, dst, maxCap, (maxT - lastt)*100);
	    //std::cout << "ret: " << lastid << std::endl;
	 }
      }
      
      auto c_map = boost::get(boost::edge_capacity, G);
      auto r_map = boost::get(boost::edge_reverse, G);
      auto rc_map = boost::get(boost::edge_residual_capacity, G);

      boost::successive_shortest_path_nonnegative_weights(G, src, dst);
      int cost = boost::find_flow_cost(G);
      int flow = 0;
      out_edge_it e, eend;
      for(boost::tie(e, eend) = boost::out_edges(boost::vertex(src,G), G); e != eend; ++e) {
          flow += c_map[*e] - rc_map[*e];
      }
      
      //std::cout << "flow " << flow << " " << expFlow << std::endl;
      assert(flow == expFlow);

      std::cout << expFlow*maxT*100 - cost << std::endl;
   }

   int idCnt = 2;
   int src = 0;
   int dst = 1;
   inline int getId(int s, int t) {
      const auto &it = iandt2id[s].find(t);
      if(it==iandt2id[s].end()) {iandt2id[s][t] = idCnt++;}
      return iandt2id[s][t];
   }
};

int main() {
   std::ios_base::sync_with_stdio(false);

   int t;
   std::cin >> t;
   for(int i=0; i<t; i++) {Test tt;}
}





