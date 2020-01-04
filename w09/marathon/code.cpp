#include <iostream>
#include <map>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
   boost::no_property, // no vertex property
   boost::property<boost::edge_weight_t, long> // interior edge weight property
   > weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;

struct Edge {
   int u,v;
   Edge(int u, int v) : u(u),v(v) {};
};
bool operator < (const Edge& e1, const Edge& e2) {
   return (e1.u < e2.u or (e1.u==e2.u and e1.v < e2.v));
}

#include <boost/graph/push_relabel_max_flow.hpp>
// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
   boost::property<boost::edge_capacity_t, long,
   boost::property<boost::edge_residual_capacity_t, long,
   boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph_f;
typedef traits::vertex_descriptor vertex_desc_f;
typedef traits::edge_descriptor edge_desc_f;

class edge_adder {
   graph_f &G;
public:
   explicit edge_adder(graph_f &G) : G(G) {}
   void add_edge(int from, int to, long capacity) {
      auto c_map = boost::get(boost::edge_capacity, G);
      auto r_map = boost::get(boost::edge_reverse, G);
      const auto e = boost::add_edge(from, to, G).first;
      const auto rev_e = boost::add_edge(to, from, G).first;
      c_map[e] = capacity;
      c_map[rev_e] = 0; // reverse edge has no capacity!
      r_map[e] = rev_e;
      r_map[rev_e] = e;
   }
};

int main() {
   std::ios_base::sync_with_stdio(false);
   
   int t;
   std::cin >> t;
   for(int tt=0; tt<t; tt++) {
      int n,m,s,f;
      std::cin >> n >> m >> s >> f;
      
      weighted_graph G(n);
      weight_map weights = boost::get(boost::edge_weight, G);
      
      std::map<Edge,long> edgeC;
      std::map<Edge,long> edgeD;

      for(int i=0; i<m; i++) {
         long a,b,c,d;
	 std::cin >> a >> b >> c >> d;
	 Edge ee(a,b);
         
	 if(a==b) {continue;}
	 if(edgeD.find(ee)!=edgeD.end()) {
	    if(edgeD[ee]<d) {continue;}
	    if(edgeD[ee]==d) {c+=edgeC[ee];}
	 }

	 edgeC[ee] = c;
	 edgeD[ee] = d;
	 edge_desc e = boost::add_edge(a,b, G).first; weights[e]=d;
      }

      std::vector<long> dist_map(n);
      boost::dijkstra_shortest_paths(G, s,
         boost::distance_map(boost::make_iterator_property_map(
         dist_map.begin(), boost::get(boost::vertex_index, G))));
      
      graph_f GF(n);
      edge_adder adder(GF);

      for(auto &it : edgeD) {
         const Edge &ee = it.first;
	 long d = it.second;

	 int u,v;
	 if(dist_map[ee.u] < dist_map[ee.v]) {
	    u = ee.u;
	    v = ee.v;
	 } else {
	    u = ee.v;
	    v = ee.u;
	 }
	 if(dist_map[v] - dist_map[u] == d) {
	    //std::cout << "found " << u << " " << v << " " << d << std::endl;
	    adder.add_edge(u,v,edgeC[ee]);
	 }
      }
      long flow = boost::push_relabel_max_flow(GF, s, f);
      
      
      //std::cout << "dist " << dist_map[s] << " " << dist_map[f] << std::endl;
      //std::cout << n << " " << m << " " << s << " " << f << std::endl;
      std::cout << flow << std::endl;
   }
}
