#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
boost::no_property, // no vertex property
boost::property<boost::edge_weight_t, int> // interior edge weight property
> weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;


struct Test {
   int n;
   Test() {
      std::cin >> n;
      
      std::vector<int> V(n*(n+1)/2);
      
      weighted_graph G(n*(n+1)/2);
      weight_map weights = boost::get(boost::edge_weight, G);
      edge_desc e;

      for(int y=0; y<n; y++) {
         for(int x=0;x<=y; x++) {
	    int v;
	    std::cin >> v;
	    int i = id(x,y);
	    V[i] = v;
            if(legal(x-1,y  )) { e = boost::add_edge(i, id(x-1,y  ), G).first; weights[e]=v; }
            if(legal(x+1,y  )) { e = boost::add_edge(i, id(x+1,y  ), G).first; weights[e]=v; }
            if(legal(x  ,y-1)) { e = boost::add_edge(i, id(x  ,y-1), G).first; weights[e]=v; }
            if(legal(x  ,y+1)) { e = boost::add_edge(i, id(x  ,y+1), G).first; weights[e]=v; }
            if(legal(x-1,y-1)) { e = boost::add_edge(i, id(x-1,y-1), G).first; weights[e]=v; }
            if(legal(x+1,y+1)) { e = boost::add_edge(i, id(x+1,y+1), G).first; weights[e]=v; }
	 }
      }
      
      int nn = boost::num_vertices(G);
      std::vector<int> dist_0(nn);
      std::vector<int> dist_1(nn);
      std::vector<int> dist_2(nn);
      boost::dijkstra_shortest_paths(G, id(0,0),
         boost::distance_map(boost::make_iterator_property_map(
         dist_0.begin(), boost::get(boost::vertex_index, G))));
      boost::dijkstra_shortest_paths(G, id(0,n-1),
         boost::distance_map(boost::make_iterator_property_map(
         dist_1.begin(), boost::get(boost::vertex_index, G))));
      boost::dijkstra_shortest_paths(G, id(n-1,n-1),
         boost::distance_map(boost::make_iterator_property_map(
         dist_2.begin(), boost::get(boost::vertex_index, G))));
      
      int minVal = V[0] + dist_0[0] + dist_1[0] + dist_2[0];

      for(int y=0; y<n; y++) {
         for(int x=0;x<=y; x++) {
	    int i = id(x,y);
	    minVal = std::min(minVal, V[i] + dist_0[i] + dist_1[i] + dist_2[i]);
	 }
      }	 

      std::cout << minVal << std::endl;
   }
   inline int id(int x, int y) {
      return y*(y+1)/2 + x;
   }
   inline bool legal(int x, int y) {
      return (x>=0 and y>=0 and y<n and x<=y);
   }
};

int main() {
   std::ios_base::sync_with_stdio(false);
   int t;
   std::cin >> t;
   for(int i=0; i<t; i++) {Test tt;}
}



