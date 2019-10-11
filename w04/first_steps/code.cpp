#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property, boost::property<boost::edge_weight_t, int> > weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;

int main() {
   std::ios_base::sync_with_stdio(false);
   

   int t(0);
   std::cin >> t;

   for(int tt=0;tt<t;tt++) {
      int n(0); int m(0);
      std::cin >> n >> m;

      weighted_graph G(n);
      weight_map weights = boost::get(boost::edge_weight, G);
      edge_desc e;

      for(int i = 0; i<m; i++) {
         int u,v,w;
	 std::cin >> u >> v >> w;
	 e = boost::add_edge(u,v,G).first;
	 weights[e] = w;
      }

      std::vector<edge_desc> mst; // mst edges
      boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));
      
      int sum(0);
      for (std::vector<edge_desc>::iterator it = mst.begin(); it != mst.end(); ++it) {
         int u = boost::source(*it, G);
         int v = boost::target(*it, G);
	 std::cout << u << " " << v << std::endl;
	 sum+=1;
      }
      std::cout << sum << std::endl;
   }
} 
