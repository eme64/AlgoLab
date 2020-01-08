#include <iostream> 
#include <vector>
#include <map>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Finite_faces_iterator Face_iterator;

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
// choose input type (input coefficients must fit)
typedef CGAL::Gmpq IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpq ET;
// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

struct Point {
   int x,y;
   Point(int x,int y) : x(x),y(y) {}
   Point() : x(0),y(0) {}
   long d2(const Point &p) const {
      long dx = p.x - x;
      long dy = p.y - y;
      return dx*dx + dy*dy;
   }
};

struct Test {
   int a,s,b,e;
   std::vector<Point> asteroid;
   std::vector<int> aDensity;
   std::vector<Point> shooting;
   std::vector<long> sRadius2;
   std::map<K::Point_2,Point> ptop;
   Test() {
      std::cin >> a >> s >> b >> e;
      asteroid.reserve(a);
      aDensity.reserve(a);
      for(int i=0; i<a; i++) {
         int x,y,d; // asteroids - d: density
	 std::cin >> x >> y >> d;
	 asteroid.push_back(Point(x,y));
	 aDensity.push_back(d);
      }
      shooting.reserve(s);
      long big = 1l << 62;
      sRadius2.resize(s, big); // default large radius
      for(int i=0; i<s; i++) {
         int x,y; // shooting positions
	 std::cin >> x >> y;
	 Point p(x,y);
	 shooting.push_back(p);
      }
      
      std::vector<K::Point_2> pts;
      pts.reserve(b);
      for(int i=0; i<b; i++) {
         int x,y; // bounty hunters
	 std::cin >> x >> y;
	 K::Point_2 p2(x,y);
	 Point p(x,y);
	 pts.push_back(p2);
	 ptop[p2] = p;
      }
      Triangulation t;
      t.insert(pts.begin(), pts.end());
      
      bool infeasible = false;
      if(b > 0) {
         for(int i=0; i<s; i++){
            const auto &ss = shooting[i];
            const auto &v = t.nearest_vertex(K::Point_2(ss.x,ss.y));
            if(!t.is_infinite(v)) {
               Point &p = ptop[v->point()];
               long dd = ss.d2(p);
	       if(dd == 0) {
	          infeasible = true;
	       }
               sRadius2[i] = dd;
               //std::cout << i << " " << dd << std::endl;
            }
         }
      }

      // set up lp.
      // variables: energy levels per shooting
      // constraints:
      //   energy budget
      //   minimum reach density per asteroid
      // cost function: irrelevant
      // check if feasible -> can do it, else cannot
      // problem: coefficients are potentially very small fractions

      // create an LP with Ax <= b, lower bound 0 and no upper bounds
      Program lp (CGAL::SMALLER, true, 0, false, 0);
      
      for(int j=0; j<a; j++) {
	 const auto &aa = asteroid[j];
         const int density = aDensity[j];
         for(int i=0; i<s; i++) {
            const auto &ss = shooting[i];
            const long r2 = sRadius2[i];
	    const long dd = aa.d2(ss);
	    if(r2 > dd) {// radius just smaller than closest b-hunter
               IT div = IT(1) / IT(std::max(1l, dd));
               lp.set_a(i,j,-div);
	    }
	 }
         lp.set_b(j,-density);
      }
      for(int i=0; i<s; i++) {
         lp.set_a(i,s,1);// energy budget
	 lp.set_c(i,1); // bogus cost
      }
      lp.set_b(s,e); // energy budget
      lp.set_c0(0);//bogus cost

      Solution s = CGAL::solve_linear_program(lp, ET());
      assert(s.solves_linear_program(lp));

      if (s.is_infeasible() or infeasible) {
         std::cout << "n" << std::endl;
      } else {
         std::cout << "y" << std::endl;
      }


   }
};

int main() {
   std::ios_base::sync_with_stdio(false);

   int t;
   std::cin >> t;
   for(int tt=0; tt<t; tt++) {Test ttt;}
}



