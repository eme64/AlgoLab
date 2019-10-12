#include <iostream>
#include <vector>
#include <map>
#include <climits>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property, boost::property<boost::edge_weight_t, int> > weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef std::map<std::pair<int,int>,edge_desc> edge_map;
typedef std::map<std::pair<int,int>,int> edge_int_map;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor vertex_desc;

int main() {
   std::ios_base::sync_with_stdio(false);
   

   int t(0);
   std::cin >> t;

   for(int tt=0;tt<t;tt++) {
      int n(0); int m(0); int spec(0); int src(0); int dst(0);
      std::cin >> n >> m >> spec >> src >> dst;
      
      weighted_graph G(n);
      edge_map em;
      weight_map weights = boost::get(boost::edge_weight, G);
      edge_desc e;
      std::vector<edge_int_map> species_distances(spec);
      edge_int_map final_weights;
      
      for(int i = 0; i<m; i++) {
         int u,v,w;
	 std::cin >> u >> v;
	 e = boost::add_edge(u,v,G).first;
	 em[std::make_pair(u,v)] = e;
	 em[std::make_pair(v,u)] = e;
         
         for(int ss=0; ss<spec; ss++) {
            std::cin >> w;
            species_distances[ss][std::make_pair(u,v)] = w;
            species_distances[ss][std::make_pair(v,u)] = w;
         }
	 final_weights[std::make_pair(v,u)] = INT_MAX;
	 final_weights[std::make_pair(u,v)] = INT_MAX;
      }
      
      // calculate private networks
      for(int ss=0; ss<spec; ss++) {
         int hive(0);
         std::cin >> hive;
         
         // prep graph with weights of this species
         for (auto it = em.begin(); it != em.end(); ++it) {
            int u = it->first.first;
            int v = it->first.second;
            e = it->second;
            weights[e] = species_distances[ss][std::make_pair(u,v)];
         }
         
         // run MST:
         std::vector<edge_desc> mst; // mst edges
         boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));
         for (std::vector<edge_desc>::iterator it = mst.begin(); it != mst.end(); ++it) {
            int u = boost::source(*it, G);
            int v = boost::target(*it, G);
            int old_weight = final_weights[std::make_pair(u,v)];
            int spec_weight = species_distances[ss][std::make_pair(u,v)];
            int new_weight = std::min(old_weight,spec_weight);
	    final_weights[std::make_pair(v,u)] = new_weight;
	    final_weights[std::make_pair(u,v)] = new_weight;
         }
     }
      
      // set weights of final graph:
      for (auto it = em.begin(); it != em.end(); ++it) {
         int u = it->first.first;
         int v = it->first.second;
         e = it->second;
         weights[e] = final_weights[std::make_pair(u,v)];
         //std::cout << u << " " << v << " " << weights[e] << std::endl;
      }
 
      // run shortest path on final graph:
      std::vector<int> dist_map(n);
      boost::dijkstra_shortest_paths(G, src,
         boost::distance_map(boost::make_iterator_property_map(
            dist_map.begin(), boost::get(boost::vertex_index, G))));
      
      // OUTPUT
      std::cout << dist_map[dst] << std::endl;
   }
} 
