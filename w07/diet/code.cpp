#include <iostream> 

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

   int n,m;
   
   while(true) {
      std::cin >> n >> m;
      if(n==0 && m==0){break;}// exit

      // create an LP with Ax <= b, lower bound 0 and no upper bounds
      Program lp (CGAL::SMALLER, true, 0, false, 0);
      
      // xj: how much of food j
      // equations:
      // sum_j xj * cji <= maxi
      // sum_j xj * cji >= minj

      for(int i=0; i<n; i++) {
         int min,max;
	 std::cin >> min >> max;

	 lp.set_b(2*i,max);
	 lp.set_b(2*i+1,-min);
      }
      for(int j=0; j<m; j++) {
         int pj;
	 std::cin >> pj;
	 lp.set_c(j,pj);
	 for(int i=0; i<n; i++) {
	    int cji;
	    std::cin >> cji;
	    lp.set_a(j,i*2,cji);
	    lp.set_a(j,i*2+1,-cji);
	 }
      }
      lp.set_c0(0);

      Solution s = CGAL::solve_linear_program(lp, ET());
      assert(s.solves_linear_program(lp));

      if(s.is_optimal()) {
         CGAL::Quotient<ET> res = s.objective_value();
	 std::cout << floor(res) << std::endl;
      } else if(s.is_unbounded()) {
         std::cout << "unbounded" << std::endl;
      } else if(s.is_infeasible()) {
         std::cout << "No such diet." << std::endl;
      } else {
         assert(false&&"else ???");
      }
   }
   
}
