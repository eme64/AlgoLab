#include <iostream>


#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Finite_faces_iterator Face_iterator;
typedef Triangulation::All_faces_iterator Face_iterator_all;

int main() {
   std::ios_base::sync_with_stdio(false);
   
   int n;
   while(true) {
      std::cin >> n;
      if(n==0) {break;}
      
      std::vector<K::Point_2> pts;
      pts.reserve(n+2);
      for(int i=0; i<n; i++){
         int x,y;
	 std::cin >> x >> y;
	 pts.push_back(K::Point_2(x,y));
      }
      int big = 1 << 27;
      pts.push_back(K::Point_2(big,big));
      pts.push_back(K::Point_2(big,-big));

      Triangulation t;
      t.insert(pts.begin(), pts.end());
      
      K::FT minDist2 = big;
      minDist2 = minDist2*minDist2*4;

      for (Face_iterator f = t.finite_faces_begin(); f != t.finite_faces_end(); ++f) {
      //for (Face_iterator_all f = t.all_faces_begin(); f != t.all_faces_end(); ++f) {
         //std::cout << t.triangle(f) << std::endl;
         K::Point_2 p0 = f->vertex(0)->point();
         K::Point_2 p1 = f->vertex(1)->point();
         K::Point_2 p2 = f->vertex(2)->point();
	 K::FT d0 = CGAL::squared_distance(p0,p1);
	 K::FT d1 = CGAL::squared_distance(p2,p1);
	 K::FT d2 = CGAL::squared_distance(p0,p2);
	 minDist2 = std::min(minDist2, std::min(d0, std::min(d1,d2)));
      }
      
      minDist2 *= 2500; // division by 2, times 100

      int approx = CGAL::to_double(CGAL::sqrt(minDist2));
      
      while(true) {
         K::FT a = approx;
	 //std::cout << "dec " << a*a << " " << minDist2 << std::endl;
         if(a*a > minDist2) {approx--;} else {break;}
      }
      while(true) {
         K::FT a = approx;
	 //std::cout << "inc " << a*a << " " << minDist2 << std::endl;
         if(a*a < minDist2) {approx++;} else {break;}
      }

      std::cout << approx << std::endl;
   }
}
