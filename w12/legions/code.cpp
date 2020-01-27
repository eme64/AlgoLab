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
};

template <typename T>
long myfloor(T v) {
   long tmp = CGAL::to_double(v);
   while(T(tmp) < v) {tmp++;}
   while(T(tmp) > v) {tmp--;}
   return tmp;
}

struct Test {
   long xs, ys, n;
   std::vector<Legion> legion;
   Test () {
      std::cin >> xs >> ys >> n;
      legion.reserve(n);
      for(int i=0; i<n; i++) {
         Legion l;
	 std::cin >> l.a >> l.b >> l.c >> l.v;
         l.det = std::sqrt(l.a*l.a + l.b*l.b);
	 legion.push_back(l);
      }
      
      Program lp (CGAL::SMALLER, false, 0, false, 0);
      const int X = 0;
      const int Y = 1;
      const int T = 2;
      
      for(int i=0; i<n; i++) {
	 Legion &l = legion[i];
	 const long dd = l.a*xs + l.b*ys + l.c;
         const bool isPos = dd > 0;
	 //std::cout << "isPos: " << isPos << std::endl;
	 const long factor = isPos ? 1: -1;
	 lp.set_a(X, i, -l.a*factor);
	 lp.set_a(Y, i, -l.b*factor);
	 lp.set_a(T, i, l.v*l.det);
	 lp.set_b(i, l.c * factor);
	 
	 //long factor = isPos ? 1: -1;
	 //lp.set_a(X, i, -l.a*factor); lp.set_a(Y, i, -l.b*factor);
	 //lp.set_b(i, -(t*l.v*l.det - l.c * factor));
      }
      lp.set_c(X, 0);
      lp.set_c(Y, 0);
      lp.set_c(T, -1);
      lp.set_c0(0);

      Solution s = CGAL::solve_linear_program(lp, ET());
      assert(s.solves_linear_program(lp));
      assert(!s.is_infeasible());
      
      CGAL::Quotient<ET> res = s.objective_value();
      
      std::cout << myfloor(-res) << std::endl;
   }

   bool isFeasible(const long t) {
      //std::cout << "try " << t << std::endl;

      Program lp (CGAL::SMALLER, false, 0, false, 0);
      const int X = 0;
      const int Y = 1;
      
      for(int i=0; i<n; i++) {
	 Legion &l = legion[i];
	 const long dd = l.a*xs + l.b*ys + l.c;
         const bool isPos = dd > 0;
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
      //std::cout << "ret " << ret << " " << t << std::endl;
      return ret;
   }
};


int main() {
   std::ios_base::sync_with_stdio(false);
   

   int t;
   std::cin >> t;
   for(int i=0; i<t; i++) {Test tt;}
}
