#include <iostream>
#include <vector>
#include <cassert>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

template <typename T>
long myfloor(T in) {
   long tmp = CGAL::to_double(in);
   while(T(tmp) < in) {tmp++;}
   while(T(tmp) > in) {tmp--;}
   return tmp;
}

struct Test {
   int n,m,c;
   std::vector<int> supply;
   std::vector<int> alc;
   std::vector<int> demand;
   std::vector<int> upper;

   Test() {
      std::cin >> n >> m >> c;
      
      Program lp (CGAL::SMALLER, true, 0, false, 0);

      supply.resize(n);
      alc.resize(n);
      demand.resize(m);
      upper.resize(m);

      for(int i=0; i<n; i++) {
         long x,y;
	 std::cin >> x >> y >> supply[i] >> alc[i];
      }

      for(int i=0; i<m; i++) {
         long x,y;
	 std::cin >> x >> y >> demand[i] >> upper[i];
	 lp.set_b(/*constr*/ n+i, demand[i]); // demand one
	 lp.set_b(/*constr*/ n+m+i, -demand[i]); // demand two
	 lp.set_b(/*constr*/ n+2*m+i, upper[i]*100); // alc
      }

      for(int w=0; w<n; w++) {
         for(int s=0; s<m; s++) {
	    int r;
	    std::cin >> r;

	    lp.set_a(var(w,s), /*constr*/w, 1); // supply constraint
	    lp.set_a(var(w,s), /*constr*/n+s, 1); // demand one
	    lp.set_a(var(w,s), /*constr*/n+m+s, -1); // demand one
	    lp.set_a(var(w,s), /*constr*/n+2*m+s, alc[w]); // alc

	    lp.set_c(var(w,s), -r);
	 }
	 lp.set_b(/*constr*/ w, supply[w]); // supply constraint
      }

      lp.set_c0(0);

      for(int i=0; i<c; i++) {
         long x,y,r;
         std::cin >> x >> y >> r;
      }

      Solution s = CGAL::solve_linear_program(lp, ET());
      assert(s.solves_linear_program(lp));
      
      if(s.is_infeasible()) {
	 std::cout << "RIOT!" << std::endl;
      } else {
         std::cout << myfloor(-s.objective_value()) << std::endl;
      }

   }

   inline int var(int w, int s) {return w*m + s;}
};

int main() {
   std::ios_base::sync_with_stdio(false);

   int t;
   std::cin >> t;
   for(int i=0; i<t; i++) {Test tt;}
}



