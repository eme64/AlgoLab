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
   
   while(true){
      int p;
      std::cin >> p;
      if(p==0){break;}

      int a,b;
      std::cin >> a >> b;
      
      int X(0),Y(1),Z(2);
      Solution s;
      if(p==1) {
         Program lp (CGAL::SMALLER, true, 0, false, 0);
         
	 // X + Y <= 4
	 lp.set_a(X,0,1);
         lp.set_a(Y,0,1);
	 lp.set_b(0,4);

         // 4X + 2Y <= ab
	 lp.set_a(X,1,4);
         lp.set_a(Y,1,2);
	 lp.set_b(1,a*b);

         // -X + Y <= 1
	 lp.set_a(X,2,-1);
         lp.set_a(Y,2,1);
	 lp.set_b(2,1);
      
         // min -by +ax
	 lp.set_c(X, a);
	 lp.set_c(Y, -b);
	 lp.set_c0(0);
	 
	 s = CGAL::solve_linear_program(lp, ET());
      } else {
         Program lp (CGAL::SMALLER, true, 0, false, 0);
         
	 lp.set_a(X,0,1);
         lp.set_a(Y,0,1);
	 lp.set_b(0,4);

	 lp.set_a(X,1,4);
         lp.set_a(Y,1,2);
         lp.set_a(Z,1,1);
	 lp.set_b(1,a*b);

	 lp.set_a(X,2,-1);
         lp.set_a(Y,2,1);
	 lp.set_b(2,1);

	 // min a*x + b*y + z
	 lp.set_c(X, -a);
	 lp.set_c(Y, -b);
	 lp.set_c(Z, -1);
	 lp.set_c0(0);
         
	 s = CGAL::solve_linear_program(lp, ET());
      }
      
      assert(s.solves_linear_program(lp));
      
      if(s.is_infeasible()) {
         std::cout << "no" << std::endl;
      } else if(!s.is_optimal()) {
	 std::cout << "unbounded" << std::endl;
      } else {
         CGAL::Quotient<ET> res = s.objective_value();
         
         long val;
         if(p==1){
            val = -to_ceil(res);
         } else {
            val = to_ceil(res);
         }
         std::cout << val << std::endl;
      }
   }
}
