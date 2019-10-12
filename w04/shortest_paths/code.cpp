#include <iostream>
#include <vector>
#include <map>
#include <climits>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property, boost::property<boost::edge_weight_t, int> > weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef std::map<std::pair<int,int>,edge_desc> edge_map;

int main() {
   std::ios_base::sync_with_stdio(false);
   

   int t(1);
   //std::cin >> t;

   for(int tt=0;tt<t;tt++) {
      int n(0); int m(0); int q(0);
      std::cin >> n >> m >> q;

      weighted_graph G(n);
      edge_map em;
      weight_map weights = boost::get(boost::edge_weight, G);
      edge_desc e;
      
      std::vector<int> lon(n);
      std::vector<int> lat(n);
      
      for(int i = 0; i<n; i++) {
         std::cin >> lon[i] >> lat[i];
      }
      
      for(int i = 0; i<m; i++) {
         int u,v,w;
	 std::cin >> u >> v >> w;
	 e = boost::add_edge(u,v,G).first;
	 em[std::make_pair(u,v)] = e;
         weights[e] = w;
      }
      
      for(int qq=0; qq<q; qq++) {
         int src(0); int dst(0);
         std::cin >> src >> dst;
         
         // SP Dijkstra
         std::vector<int> dist_map(n);
         boost::dijkstra_shortest_paths(G, src,
            boost::distance_map(boost::make_iterator_property_map(
               dist_map.begin(), boost::get(boost::vertex_index, G))));
         if(dist_map[dst]<INT_MAX) {
         std::cout << dist_map[dst] << std::endl;
         } else {
         std::cout << "unreachable" << std::endl;
         }
      }
   }
} 
