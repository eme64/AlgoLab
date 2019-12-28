#include <iostream>
#include <math.h>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

template<class T>
int floor(const T &t) {
   int res = CGAL::to_double(t);
   T tres = res;
   if(tres > t) {res--;}
   return res;
}

int main() {
   std::ios_base::sync_with_stdio(false);

   while(true) {
      int n,d;
      std::cin >> n;
      if(n==0) {break;}

      std::cin >> d;

      Program lp (CGAL::SMALLER, false, 0, false, 0);

      // idea: d variables for coordinate, +1 for radius
      // could normalize ai
      // nai*x + r <= bi
      // ai*x + r*norm <= bi
      
      for(int i=0; i<n; i++) {
	 long norm = 0;
         for(int j=0; j<d; j++) {
	    int aij;
	    std::cin >> aij;
	    norm+= aij*aij;

	    lp.set_a(j,i,aij);
	 }
	 norm = std::sqrt(norm);
	 lp.set_a(d,i,norm);
	 int bi;
	 std::cin >> bi;
	 lp.set_b(i,bi);
      }
      lp.set_l(d,true,0);
      lp.set_c(d,-1);
      lp.set_c0(0);

      // solve the program, using ET as the exact type
      Solution s = CGAL::solve_linear_program(lp, ET());
      assert(s.solves_linear_program(lp));
      
      if(s.is_infeasible()) {
         std::cout << "none" << std::endl;
      } else if(s.is_optimal()) {
	 CGAL::Quotient<ET> res = -s.objective_value();
         std::cout << floor(res) << std::endl;
      } else if(s.is_unbounded()) {
         std::cout << "inf" << std::endl;
      } else {
         std::cout << s; 
         assert(false&&"else ???");
      }
   }
   
}
