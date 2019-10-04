#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;

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
      P intersection;
      K::FT d2;

      for(int i=0; i<n; i++) {
         long r(0),s(0),t(1),u(1);
	 std::cin >> r >> s >> t >> u;
	 S seg = S(P(r,s),P(t,u));
	 if(CGAL::do_intersect(ray,seg)) {
            auto ii(CGAL::intersection(ray,seg));
	    if(const P* pp = boost::get<P>(&*ii)) {
	       K::FT d2s = CGAL::squared_distance(P(x,y),*pp);
	       if(not doIntersect or d2 > d2s) {
	          d2 = d2s;
		  intersection = *pp;
	       }

	    } else if(const S* ss = boost::get<S>(&*ii)) {
	       K::FT d2src = CGAL::squared_distance(P(x,y),ss->source());
	       K::FT d2dst = CGAL::squared_distance(P(x,y),ss->target());
               if(not doIntersect or d2 > d2src) {
	          d2 = d2src;
		  intersection = ss->source();
	       }
	       if(d2 > d2dst) {
	          d2 = d2dst;
		  intersection = ss->target();
	       }
	    }
            
            doIntersect = true;
	 }
      }

      if(doIntersect) {
	 std::cout << (long)std::floor(CGAL::to_double(intersection.x()))
		   << " "
		   << (long)std::floor(CGAL::to_double(intersection.y())) << std::endl;
      } else {
         std::cout << "no" << std::endl;
      }
   }}
