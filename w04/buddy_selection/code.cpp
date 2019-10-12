#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <climits>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;

int main() {
   std::ios_base::sync_with_stdio(false);
   

   int t(0);
   std::cin >> t;

   for(int tt=0;tt<t;tt++) {
      int n(0); int c(0); int f(0);;
      std::cin >> n >> c >> f;
      
      graph G(n);
      std::map<std::string,int> stringMap;
      int stringMapCounter(0);
      
      std::vector<std::set<int>> sets(n);
      for(int i = 0; i<n; i++) {
         std::string word;
	 
         for(int cc=0; cc<c; cc++) {
            std::cin >> word;
            auto search = stringMap.find(word);
            if(search == stringMap.end()) {stringMap[word] = stringMapCounter++;}
            int el(stringMap[word]);
            sets[i].insert(el);
         }
         for(int j=0; j<i; j++) {
            int cnt(0);
            for(auto el : sets[i]) {
               if(sets[j].find(el) != sets[j].end()) {cnt++;}
            }
            if(cnt>f) {
               boost::add_edge(i,j,G);
            }
         }
      }
      
      // run matching on it:
      std::vector<vertex_desc> mate_map(n);
      const vertex_desc NULL_VERTEX = boost::graph_traits<graph>::null_vertex();
      boost::edmonds_maximum_cardinality_matching(G,
         boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
      int matching_size = boost::matching_size(G,
         boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
      
      // OUTPUT
      std::cout << ((matching_size*2<n)? "optimal" : "not optimal") << std::endl;
   }
} 
