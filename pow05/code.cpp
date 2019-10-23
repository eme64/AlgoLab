#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>

#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
           boost::no_property,
           boost::property<boost::edge_weight_t, int> > weighted_graph;

typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor vertex_desc;


int main() {
   std::ios_base::sync_with_stdio(false);
   
   std::vector<int> teleportNetwork;
   
   int t(0);
   std::cin >> t;
   for(int tt=0;tt<t;tt++) {
      int n(0); int m(0); int k(0); int T(0);
      std::cin >> n >> m >> k >> T;
      
      teleportNetwork.resize(T);
      for(int i=0;i<T;i++) {std::cin >> teleportNetwork[i];}
      
      weighted_graph G(n);
      weight_map weights = boost::get(boost::edge_weight, G);
      edge_desc e;
      for(int i=0;i<m;i++) {
         int u,v,c;
         std::cin >> u >> v >> c;
         e = boost::add_edge(v,u,G).first; weights[e] = c;
      }
      std::vector<int> scc_map(n);
      int nscc = boost::strong_components(G,
         boost::make_iterator_property_map(scc_map.begin(), boost::get(boost::vertex_index, G)));
      
      std::vector<int> telCompSize(nscc); // for each component list count members from telNet
      
      //std::cout << "v comp" << std::endl;
      for(int i=0; i<T; i++) {
         int v = teleportNetwork[i];
         int comp = scc_map[v];
         telCompSize[comp]++;
         //std::cout << teleportNetwork[i] << " " << scc_map[teleportNetwork[i]] << std::endl;
      }
      
      // add teleporation nodes for each component:
      for(int i=0; i<T; i++) {
         int v = teleportNetwork[i];
         int comp = scc_map[v];
         int telNode = n+comp;
         
         e = boost::add_edge(v,telNode,G).first; weights[e] = 0;
         e = boost::add_edge(telNode,v,G).first; weights[e] = telCompSize[comp]-1;
      }
      
      // run shortest path on constructed graph:
      int nn = boost::num_vertices(G);
      //std::cout << "sizes: " << n << " " << nn << std::endl;
      std::vector<int> dist_map(nn);
      
      boost::dijkstra_shortest_paths(G, n-1,
         boost::distance_map(boost::make_iterator_property_map(
            dist_map.begin(), boost::get(boost::vertex_index, G))));
      
      // read out times:
      int minTime = INT_MAX;
      //std::cout << "warehouse dist" << std::endl;
      for(int i=0;i<k;i++) {
         minTime = std::min(minTime, dist_map[i]);
         //std::cout << i << " " << dist_map[i] << std::endl;
      }
      
      if(minTime <= 1000000) {
         std::cout << minTime << std::endl;
      } else {
         std::cout << "no" << std::endl;
      }
      
   }
   
   return 0;
}
