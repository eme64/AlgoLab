#include <iostream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef K::Point_2 P;
typedef K::Segment_2 S;
typedef K::Ray_2 R;

int main() {
   std::ios_base::sync_with_stdio(false);

   while(true) {
      int n(0);
      std::cin >> n;
      if(n==0) {break;}

      long x(0),y(0),a(1),b(1);
      std::cin >> x >> y >> a >> b;
      
      R ray = R(P(x,y),P(a,b));
      
      bool doIntersect(false);

      for(int i=0; i<n; i++) {
         long r(0),s(0),t(1),u(1);
	 std::cin >> r >> s >> t >> u;
	 S seg = S(P(r,s),P(t,u));
	 if(doIntersect or CGAL::do_intersect(ray,seg)) {
	    doIntersect = true;
	 }
      }


      std::cout << ((doIntersect)?"yes":"no") << std::endl;
   }
}
