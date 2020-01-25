#include <iostream>
#include <map>
#include <vector>
#include <cassert>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
// choose input type (input coefficients must fit)
typedef long IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

struct Legion {
   long a,b,c,v,det;
   Legion() {}
   Legion(long a, long b, long c, long v) : a(a), b(b), c(c), v(v) {
      det = std::sqrt(a*a + b*b);
   }
};

struct Test {
   int xs, ys, n;
   std::vector<Legion> legion;
   Test () {
      std::cin >> xs >> ys >> n;
      legion.reserve(n);
      for(int i=0; i<n; i++) {
         Legion l;
	 std::cin >> l.a >> l.b >> l.c >> l.v;
	 legion.push_back(l);
      }
      
      long lowd = 0;
      long highd = 1;

      while(isFeasible(highd)) {lowd = highd; highd = highd*2;}
      while(lowd+1<highd) {
	 //std::cout << " - " << lowd << " " << highd << std::endl;
         long m = (lowd + highd)/2;
	 if(isFeasible(m)) {
	    lowd = m;
	 } else {
            highd = m;
	 }
      }
      
      if(not isFeasible(lowd)) {std::cout << "lowd too high" << std::endl;}
      if(isFeasible(lowd+1)) {std::cout << "lowd too low" << std::endl;}
  
      std::cout << lowd << std::endl;
   }

   bool isFeasible(const long t) {
      //std::cout << "try " << t << std::endl;

      Program lp (CGAL::SMALLER, false, 0, false, 0);
      const int X = 0;
      const int Y = 1;
      
      for(int i=0; i<n; i++) {
	 Legion &l = legion[i];
         const bool isPos = (l.a*xs + l.b*ys + l.c) > 0l;
	 //std::cout << "isPos: " << isPos << std::endl;
	 const long factor = isPos ? 1: -1;
	 lp.set_a(X, i, -l.a*factor); lp.set_a(Y, i, -l.b*factor);
	 lp.set_b(i, -(t*l.v*l.det - l.c * factor));
	 //long factor = isPos ? 1: -1;
	 //lp.set_a(X, i, -l.a*factor); lp.set_a(Y, i, -l.b*factor);
	 //lp.set_b(i, -(t*l.v*l.det - l.c * factor));
      }
      lp.set_c(X, 1);
      lp.set_c(Y, 1);
      lp.set_c0(0);

      Solution s = CGAL::solve_linear_program(lp, ET());
      assert(s.solves_linear_program(lp));
      
      bool ret = !s.is_infeasible();
      //std::cout << "ret " << ret << std::endl;
      return ret;
   }
};


int main() {
   std::ios_base::sync_with_stdio(false);
   
   assert(false);

   int t;
   std::cin >> t;
   for(int i=0; i<t; i++) {Test tt;}
}
