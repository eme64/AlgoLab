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

int main() {
   std::ios_base::sync_with_stdio(false);
   
   int t;
   std::cin >> t;

   for(int tt=0; tt<t; tt++){
      int n,m,h,w;
      std::cin >> n >> m >> h >> w;
      
      
      std::vector<Element> elements;
      elements.reserve(n+m);

      for(int i=0; i<n; i++) {
         int x,y;
	 std::cin >> x >> y;
	 elements.push_back(Element(i,x*2,y*2));
      }

      for(int i=0; i<m; i++) {
         int x,y;
	 std::cin >> x >> y;
	 elements.push_back(Element(-1,x*2,y*2));
      }

      std::sort(elements.begin(), elements.end());
      
      Program lp (CGAL::SMALLER, true, 1, false, 0);
       
      const int imax = elements.size()-1;
      int eqNum = 0;

      for(int i=0; i<imax; i++) {
         const Element& e1 = elements[i];
         const Element& e2 = elements[i+1];
         
	 if(e1.variable == -1 && e2.variable == -1) {
            // ignore
	 } else if(e1.variable >= 0 && e2.variable >= 0) {
	    // mutual limitation
	    int dist = std::abs(e1.x - e2.x);
	    lp.set_a(e1.variable, eqNum, w);
	    lp.set_a(e2.variable, eqNum, w);
            lp.set_b(eqNum, dist);
	    eqNum++;
	 } else if(e1.variable >= 0 && e2.variable == -1) {
            // one-sided
	    int dist = std::abs(e1.x - e2.x) - w;
	    lp.set_a(e1.variable, eqNum, w);
            lp.set_b(eqNum, dist);
	    eqNum++;
	 } else if(e1.variable == -1 && e2.variable >= 0) {
            // one-sided
	    int dist = std::abs(e1.x - e2.x) - w;
	    lp.set_a(e2.variable, eqNum, w);
            lp.set_b(eqNum, dist);
	    eqNum++;
	 } else {
	    assert(false);
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
