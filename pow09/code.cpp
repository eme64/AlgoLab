#include <cassert>
#include <iostream>
#include <vector>
#include <algorithm>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef long IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

struct Element {
   int variable; // -1 if old poster
   int x,y;
   Element(int _variable, int _x, int _y) : variable(_variable), x(_x), y(_y) {}
};

bool operator < (const Element& e1, const Element& e2) {
   return (e1.x < e2.x);
}

template<typename T>
long to_ceil(const T& t) {
   const double aprox = CGAL::to_double(t);
   long res = aprox;
   
   if(T(res) < t) {
      res++;
   }
   assert(T(res) >= t && T(res) < t+1);
   return res;
}

// Idea:
//
// separate into
//  max bounds per new poster from old posters - precomupte min bound
//  mutual bounds of new posters - n^2 in lp

struct Bound { // struct to easily find minimum Bound
   long dist;
   long side; // h or w value
   Bound(int d, int s) : dist(d), side(s) {};
};
bool operator < (const Bound& e1, const Bound& e2) {
   return (e1.dist*e2.side < e2.dist*e1.side);// predicate?
}

int main() {
   std::ios_base::sync_with_stdio(false);
   
   int t;
   std::cin >> t;

   for(int tt=0; tt<t; tt++){
      int n,m,h,w;
      std::cin >> n >> m >> h >> w;
      
      
      std::vector<Element> oldElements;
      std::vector<Element> newElements;
      oldElements.reserve(m);
      newElements.reserve(n);

      for(int i=0; i<n; i++) {
         int x,y;
	 std::cin >> x >> y;
	 newElements.push_back(Element(i,x*2,y*2));
      }

      for(int i=0; i<m; i++) {
         int x,y;
	 std::cin >> x >> y;
	 oldElements.push_back(Element(-1,x*2,y*2));
      }

      //std::sort(elements.begin(), elements.end());
      
      Program lp (CGAL::SMALLER, true, 1, false, 0);
       
      //const int imax = elements.size()-1;
      int eqNum = 0;

      for(int i=0;i<n;i++) {
         const Element& en = newElements[i];
         
	 Bound bound = Bound(1 << 25, -1);

	 for(int j=0;j<m;j++) {
	    const Element& eo = oldElements[j];
            long dx = std::abs(en.x - eo.x);
            long dy = std::abs(en.y - eo.y);

	    if(dx*h <= dy*w) {// predicate?
	       bound = std::min( bound, Bound(std::abs(dy-h),h));
	    } else {
	       bound = std::min( bound, Bound(std::abs(dx-w),w));
	    }
	 }
	 lp.set_a(en.variable, eqNum, bound.side); // set upper bound on variable for old
         lp.set_b(eqNum, bound.dist);
         eqNum++;
      
         for(int j=0;j<i;j++) { // for each other new mutual bounds
	    const Element& eo = newElements[j];
            
	    long dx = std::abs(en.x - eo.x);
            long dy = std::abs(en.y - eo.y);

	    if(dx*h <= dy*w) {// predicate?
	       bound = Bound(dy,h);

	    } else {
	       bound = Bound(dx,w);
	    }
            lp.set_a(en.variable, eqNum, bound.side);
            lp.set_a(eo.variable, eqNum, bound.side);
            lp.set_b(eqNum, bound.dist);
            eqNum++;
	 }
      }
      
      for(int i=0; i<n; i++) {
         lp.set_c(i, -(2*w+2*h));
      }
      lp.set_c0(0);

      Solution s = CGAL::solve_linear_program(lp, ET());
      assert(s.solves_linear_program(lp));

      //std::cout << s;

      CGAL::Quotient<ET> res = -s.objective_value();

      long val = to_ceil(res);
      std::cout << val << std::endl;
      //std::cout << CGAL::to_double(res) << std::endl;
      
      //std::cout << eqNum << std::endl;
   }
}
