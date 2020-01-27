#include <iostream>
#include <cassert>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> V;
typedef CGAL::Triangulation_data_structure_2<V> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;
typedef Triangulation::Finite_faces_iterator Face_iterator;
typedef K::Point_2 P;


#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS> graph;
typedef boost::graph_traits<graph>::edge_iterator edge_it;
typedef boost::graph_traits<graph>::out_edge_iterator out_edge_it;

struct Test {
   int n,m,r;
   K::FT r2;
   std::vector<std::pair<P,int>> p;
   Test() {
      std::cin >> n >> m >> r;
      r2 = K::FT(r) * K::FT(r);
      p.reserve(n+4);
      for(int i=0; i<n; i++) {
         // stations
	 int x,y;
	 std::cin >> x >> y;
	 P pp(x,y);
	 p.push_back(std::make_pair(pp,i));
      }
      int big = 1 << 26;
      { P pp(-big,-big); p.push_back(std::make_pair(pp,-1)); }
      { P pp( big, big); p.push_back(std::make_pair(pp,-1)); }
      { P pp( big,-big); p.push_back(std::make_pair(pp,-1)); }
      { P pp(-big, big); p.push_back(std::make_pair(pp,-1)); }

      Triangulation t;
      t.insert(p.begin(), p.end());
      
      graph G(n);

      bool interference = false;

      for(auto e = t.finite_edges_begin(); e!=t.finite_edges_end(); e++) {
         const auto &v1 = e->first->vertex((e->second + 1) % 3);
         const auto &v2 = e->first->vertex((e->second + 2) % 3);
	 const int i1 = v1->info();
	 const int i2 = v2->info();
	 if(i1>=0 and i2>=0 and r2 >= CGAL::squared_distance(v1->point(),v2->point())) {
             boost::add_edge(i1,i2,G);
             boost::add_edge(i2,i1,G);
	 }
      }
      
      // idea: take graph, visit, try coloring, if conflict -> exit
      // if no conflict: for both color verify with new triangulation, check distances there
      
      if(!interference) {
         std::vector<int> color(n, 0); // set to 1,2

         //BFS
         std::vector<int> queue;
         queue.reserve(n);
         int qIndex = 0;
         for(int i=0; i<n; i++) {
            if(color[i]==0) {
               queue.push_back(i);
               color[i] = 1;
               while(qIndex < queue.size()) {
                  int curr = queue[qIndex++];
                  // visit children:
                  out_edge_it oe_beg, oe_end;
                  for (boost::tie(oe_beg, oe_end) = boost::out_edges(curr, G); oe_beg != oe_end; ++oe_beg) {
                     assert(boost::source(*oe_beg, G) == curr);
           	  int j = boost::target(*oe_beg, G);

           	  if(color[j] == 0) {queue.push_back(j);}
           	  if(color[j] == color[curr]) {interference = true;}
           	  color[j] = 3-color[curr];
                  }
               }
            }
         }

         if(!interference) {
            std::vector<P> one;
            std::vector<P> two;
            one.reserve(n);
            two.reserve(n);
            for(int i=0; i<n;i++) {
               if(color[i] == 1) {
                  one.push_back(p[i].first);
               } else {
                  two.push_back(p[i].first);
               }
            }
            if(!noContact(one) or !noContact(two)) {interference = true;}
         }
      }

      //std::cout << "int " << interference << std::endl;
      
      std::vector<int> scc_map(n);
      int nscc = boost::strong_components(G,
                   boost::make_iterator_property_map(scc_map.begin(), boost::get(boost::vertex_index, G)));
      
      for(int i=0; i<m; i++) {
         // clues
         int x1,y1,x2,y2;
         std::cin >> x1 >> y1 >> x2 >> y2;
         P pa(x1,y1);
         P pb(x2,y2);
         K::FT d2 = CGAL::squared_distance(pa,pb);

         if(interference) {
            std::cout << "n"; // network bad
         } else if(d2 <= r2) {
            std::cout << "y"; // direct
         } else {
            const auto &va = t.nearest_vertex(pa);
            const auto &vb = t.nearest_vertex(pb);
	    int ia = va->info();
	    int ib = vb->info();
	    assert(ia>=0 and ib>=0);
            if(scc_map[ia] == scc_map[ib]
               && CGAL::squared_distance(va->point(),pa) <= r2
               && CGAL::squared_distance(vb->point(),pb) <= r2) {
               std::cout << "y";
            } else {
               std::cout << "n";
            }
         }
      }
      std::cout << std::endl;
   }

   bool noContact(std::vector<P> &pts) {
      Triangulation t;
      int big = 1 << 26;
      { P pp(-big,-big); pts.push_back(pp);}
      { P pp( big, big); pts.push_back(pp);}
      { P pp(-big, big); pts.push_back(pp);}
      { P pp( big,-big); pts.push_back(pp);}
      t.insert(pts.begin(), pts.end());
      
      for(auto e = t.finite_edges_begin(); e!=t.finite_edges_end(); e++) {
         const auto &v1 = e->first->vertex((e->second + 1) % 3);
         const auto &v2 = e->first->vertex((e->second + 2) % 3);
	 
	 if(r2 >= CGAL::squared_distance(v1->point(),v2->point())) {
	    return false;
	 }
      }
     
      return true;
   }
};

int main() {
   std::ios_base::sync_with_stdio(false);

   int t;
   std::cin >> t;
   for(int i=0; i<t; i++) {
      Test tt;
   }
}
