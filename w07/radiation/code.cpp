#include <iostream>
#include <vector>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
// choose input type (input coefficients must fit)
typedef CGAL::Gmpz IT; // int
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;


struct Coordinate {
   IT x,y,z;
   Coordinate(IT x, IT y, IT z) : x(x),y(y),z(z) {}
};

typedef std::vector<Coordinate> VC;

bool check(VC &healthy, VC &tumor, int d) {
   Program lp (CGAL::SMALLER, false, 0, false, 0); 
   
   //std::cout << "prep" << std::endl;
   int eq = 0;
   VC exp(d+1,Coordinate(1,1,1));
   for(Coordinate &h : healthy) {
      for(int x=1; x<=d; x++) {
         exp[x] = Coordinate(exp[x-1].x*h.x, exp[x-1].y*h.y, exp[x-1].z*h.z);
      }
      int i=1;
      for(int x=0; x<=d; x++) {
         for(int y=0; y<=d; y++) {
	    for(int z=0; z<=d; z++) {
	       if(x+y+z<=d) {
		  lp.set_a(i,eq,exp[x].x*exp[y].y*exp[z].z);
	          i++;
	       }
	    }
	 }
      }
      lp.set_a(0,eq,1);// slack
      lp.set_b(eq,0);
      eq++;
   }
   for(Coordinate &h : tumor) {
      for(int x=1; x<=d; x++) {
         exp[x] = Coordinate(exp[x-1].x*h.x, exp[x-1].y*h.y, exp[x-1].z*h.z);
      }
      int i=1;
      for(int x=0; x<=d; x++) {
         for(int y=0; y<=d; y++) {
	    for(int z=0; z<=d; z++) {
	       if(x+y+z<=d) {
		  lp.set_a(i,eq,-exp[x].x*exp[y].y*exp[z].z);
	          i++;
	       }
	    }
	 }
      }
      lp.set_a(0,eq,1);// slack
      lp.set_b(eq,0);
      eq++;
   }
   
   lp.set_l(0,true,0); // positive slack
   lp.set_c(0,-1); // maximize slack
   lp.set_c0(0);
   
   // solve the program, using ET as the exact type
   //std::cout << "solve" << std::endl;
   CGAL::Quadratic_program_options options;
   options.set_pricing_strategy(CGAL::QP_BLAND);
   Solution s = CGAL::solve_linear_program(lp, ET(), options);
   assert(s.solves_linear_program(lp));
   
   if(s.is_infeasible()) {
      //std::cout << "bad " << d << std::endl;
      return false;
   } else if(s.is_optimal()) {
      //std::cout << "optimal " << d << " " << s.objective_value() << std::endl;
      CGAL::Quotient<ET> res = s.objective_value();
      ET zero = 0;
      if(res == zero) {
         return false;
      } else {
	 assert(false&&"why?");
	 return true;
      }
   } else if(s.is_unbounded()) {
      //std::cout << "ok " << d << std::endl;
      return true;
   } else {
      std::cout << s;
      assert(false&&"bad solution ???");
      return false;
   }
}

int main() {
   std::ios_base::sync_with_stdio(false);


   int ttt;
   std::cin >> ttt;
   for(int tt=0; tt<ttt; tt++) {
      int h,t;
      std::cin >> h >> t;

      //std::cout << "# # # " << h << " " << t << std::endl;

      // take idea from ex2 cubic separation in slides
      // {...} + slack <= j
      // {...} - slack >= j
      // slack >= 0
      // maximize on slack, check that > 0
      
      VC healthy;
      VC tumor;

      for(int i=0; i<h; i++) {
         int x,y,z;
	 std::cin >> x >> y >> z;
	 healthy.push_back(Coordinate(x,y,z));
      }
      for(int i=0; i<t; i++) {
         int x,y,z;
	 std::cin >> x >> y >> z;
	 tumor.push_back(Coordinate(x,y,z));
      }
      
      bool success = false;
      for(int i=0; i<=30; i++) {
         bool res = check(healthy, tumor, i);
	 if(res) {
	    std::cout << i << std::endl;
	    success = true;
	    break;
	 }
      }
      
      if(!success) {
         std::cout << "Impossible!" << std::endl;
      }
   }
}




