#include <iostream>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Finite_faces_iterator Face_iterator;


int main() {
   std::ios_base::sync_with_stdio(false);

   while(true) {
      int n,m;
      std::cin >> n;
      if(n==0) {break;}
      
      std::vector<K::Point_2> pts;
      pts.reserve(n);
      for(int i=0; i<n; i++) {
         int x,y;
	 std::cin >> x >> y;
         pts.push_back(K::Point_2(x, y));
      }

      Triangulation t;
      t.insert(pts.begin(), pts.end());
      
      std::cin >> m;
      for(int i=0; i<m; i++) {
         int x,y;
	 std::cin >> x >> y;
	 K::Point_2 q(x,y);
         K::Point_2& p = t.nearest_vertex(q)->point();
	 std::cout << (long)CGAL::squared_distance(p,q) << std::endl;
      }
   }
}
