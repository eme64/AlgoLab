#include <iostream>
#include <vector>
#include <cassert>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Finite_faces_iterator Face_iterator;


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

   std::vector<long> xw;
   std::vector<long> yw;
   std::vector<long> xs;
   std::vector<long> ys;

   std::vector<bool> wvc;
   std::vector<bool> svc;
   
   std::vector<int> revenue;

   Test() {
      std::cin >> n >> m >> c;
      
      Program lp (CGAL::SMALLER, true, 0, false, 0);

      supply.resize(n);
      alc.resize(n);
      demand.resize(m);
      upper.resize(m);
      
      xw.resize(n);
      yw.resize(n);
      xs.resize(m);
      ys.resize(m);

      wvc.resize(n*100,false);
      svc.resize(m*100,false);
      
      revenue.resize(n*m);

      std::vector<K::Point_2> pts;
      pts.reserve(n+m);

      for(int i=0; i<n; i++) {
         long x,y;
	 std::cin >> x >> y >> supply[i] >> alc[i];
         pts.push_back(K::Point_2(x, y));
	 xw[i] = x;
	 yw[i] = y;
      }

      for(int i=0; i<m; i++) {
         long x,y;
	 std::cin >> x >> y >> demand[i] >> upper[i];
	 lp.set_b(/*constr*/ n+i, demand[i]); // demand one
	 lp.set_b(/*constr*/ n+m+i, -demand[i]); // demand two
	 lp.set_b(/*constr*/ n+2*m+i, upper[i]*100); // alc
         pts.push_back(K::Point_2(x, y));
	 xs[i] = x;
	 ys[i] = y;
      }

      Triangulation t;
      t.insert(pts.begin(), pts.end());

      for(int w=0; w<n; w++) {
         for(int s=0; s<m; s++) {
	    int r;
	    std::cin >> r;
	    revenue[w*m + s] = r;

	    lp.set_a(var(w,s), /*constr*/w, 1); // supply constraint
	    lp.set_a(var(w,s), /*constr*/n+s, 1); // demand one
	    lp.set_a(var(w,s), /*constr*/n+m+s, -1); // demand one
	    lp.set_a(var(w,s), /*constr*/n+2*m+s, alc[w]); // alc
	 }
	 lp.set_b(/*constr*/ w, supply[w]); // supply constraint
      }

      lp.set_c0(0);
      
      int cCnt = 0;
      for(int i=0; i<c; i++) {
         long x,y,r;
         std::cin >> x >> y >> r;
	 const auto &v = t.nearest_vertex(K::Point_2(x,y));
	 if(CGAL::squared_distance(v->point(), K::Point_2(x,y)) <= K::FT(r*r)) {
	    for(int w=0; w<n; w++) {
	       const long dx = xw[w] - x;
	       const long dy = yw[w] - y;
	       if(dx*dx + dy*dy < r*r) { wvc[w*100 + cCnt] = true; }
	    }
	    for(int s=0; s<m; s++) {
	       const long dx = xs[s] - x;
	       const long dy = ys[s] - y;
	       if(dx*dx + dy*dy < r*r) { svc[s*100 + cCnt] = true; }
	    }
	    cCnt++;

	    assert(cCnt <= 100);
	 }
      }

      for(int w=0; w<n; w++) {
         for(int s=0; s<m; s++) {
	    int r = 100*revenue[w*m + s];
            
	    for(int i=0; i<cCnt; i++) {
	       bool c1 = wvc[w*100 + i];
	       bool c2 = svc[s*100 + i];

	       if(c1 ^ c2) {r-=1;}
	    }
	    
	    lp.set_c(var(w,s), -r);
	 }
      }


      Solution s = CGAL::solve_linear_program(lp, ET());
      assert(s.solves_linear_program(lp));
      
      if(s.is_infeasible()) {
	 std::cout << "RIOT!" << std::endl;
      } else {
         std::cout << myfloor(-s.objective_value()/ET(100)) << std::endl;
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



