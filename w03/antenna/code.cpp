#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <vector>

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef CGAL::Min_circle_2_traits_2<K> Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;

typedef K::Point_2 P;
typedef K::Segment_2 S;
typedef K::Ray_2 R;

long makeCeil(K::FT v) {
   double f = CGAL::to_double(v);
   long res = (long)std::ceil(f);
   while(K::FT(res-1)>v) {res--;}
   while(K::FT(res)<v) {res++;}
   if(K::FT(res-1)>v or K::FT(res)<v) {
      throw std::runtime_error("rounding error");
   }
   return res;
}

int main() {
   std::ios_base::sync_with_stdio(false);

   while(true) {
      int n(0);
      std::cin >> n;
      if(n==0) {break;}
      
      std::vector<P> points;
      points.reserve(n);

      for(int i=0; i<n; i++) {
         long x,y;
	 std::cin >> x >> y;
	 points.push_back(P(x,y));
      }
      Min_circle mc(points.begin(), points.end(), true);//do randomize
      K::FT radius2 = mc.circle().squared_radius();
      K::FT radius = CGAL::sqrt(radius2);
      std::cout << makeCeil(radius) << std::endl;
   }}
