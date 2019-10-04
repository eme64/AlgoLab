#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;

typedef K::Point_2 P;
typedef K::Segment_2 S;
typedef K::Ray_2 R;

long makeFloor(K::FT v) {
   double f = CGAL::to_double(v);
   long res = (long)std::floor(f);
   while(K::FT(res)>v) {res--;}
   while(K::FT(res+1)<v) {res++;}
   if(K::FT(res)>v or K::FT(res+1)<v) {
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
	    } else {
	       throw std::runtime_error("strange segment intersection");
	    }
            
            doIntersect = true;
	 }
      }
      if(doIntersect) {
	 long outX = makeFloor(intersection.x());
	 long outY = makeFloor(intersection.y());
         
         if(K::FT(outX)>intersection.x() or K::FT(outX+1)<intersection.x()) {
	    if(intersection.x() > 0) throw std::runtime_error("rounding error");
	 }

	 std::cout << outX << " " << outY << std::endl;
      } else {
         std::cout << "no" << std::endl;
      }
   }}
