// idea:
//
// flow.
// G1: input at police stations, 1 per station, edge capacity max
// G2: output at police station, 1 per station, edge capapcity 1
// connect G1->G2 at picture

#include <iostream>
#include <vector>
#include <map>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
   boost::property<boost::edge_capacity_t, long,
   boost::property<boost::edge_residual_capacity_t, long,
   boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;
typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

class edge_adder {
graph &G;
public:
explicit edge_adder(graph &G) : G(G) {}
void add_edge(int from, int to, long capacity) {
auto c_map = boost::get(boost::edge_capacity, G);
auto r_map = boost::get(boost::edge_reverse, G);
const auto e = boost::add_edge(from, to, G).first;
const auto rev_e = boost::add_edge(to, from, G).first;
c_map[e] = capacity;
c_map[rev_e] = 0; // reverse edge has no capacity!
r_map[e] = rev_e;
r_map[rev_e] = e;
//std::cout << from << " " << to << " " << capacity << std::endl;
}
};

struct Test {
   int n,m,k,l;
   std::vector<int> police;
   std::vector<int> photo;
   std::vector<int> M;
   Test() {
      std::cin >> n >> m >> k >> l;
      police.resize(n,0);
      photo.resize(n,0);
      M.resize(n*n,0);

      for(int i=0; i<k; i++) {
         int pp;
	 std::cin >> pp;
	 police[pp]++;
      }
      for(int i=0; i<l; i++) {
         int pp;
	 std::cin >> pp;
	 photo[pp]++;
      }
      
      for(int i=0; i<m; i++) {
         int x,y;
	 std::cin >> x >> y;
	 M[x*n + y]++;
      }

      graph G(2*n+2);
      int src = 2*n;
      int dst = 2*n+1;
      edge_adder adder(G);

      for(int x=0; x<n; x++) {
	 if(police[x]>0) { adder.add_edge(src,x,police[x]); }
	 if(police[x]>0) { adder.add_edge(n+x,dst,police[x]); }
	 if(photo[x]>0) { adder.add_edge(x,n+x,photo[x]); }
	 for(int y=0; y<n; y++) {
	    if(M[x*n + y]>0) { adder.add_edge(x,y,k); }
	    if(M[x*n + y]>0) { adder.add_edge(n+x,n+y,M[x*n + y]); }
	 }
      }
      
      long flow = boost::push_relabel_max_flow(G, src, dst);
      std::cout << flow << std::endl;
   }
};

int main() {
   std::ios_base::sync_with_stdio(false);
   int t;
   std::cin >> t;
   for(int tt=0; tt<t; tt++) {
      Test ttt;
   }
}
