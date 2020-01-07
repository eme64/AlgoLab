// idea:
// bin-search O(logn)
// build delaunay-triangulation O(n logn)
// for each person: find closest light -> check if overlaps ?

#include <iostream>
#include <vector>
#include <map>
#include <cassert>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Finite_faces_iterator Face_iterator;


struct Participant {
   int x,y;
   long r;
};

struct Case {
   int m,n;
   long h;
   std::vector<Participant> participants;
   std::vector<K::Point_2> lamps;
   Case() {
      std::cin >> m >> n;
      
      participants.reserve(m);
      for(int i=0; i<m; i++) {
         Participant p;
	 std::cin >> p.x >> p.y >> p.r;
	 participants.push_back(p);
      }

      std::cin >> h;
      lamps.reserve(n);
      for(int i=0; i<n; i++) {
         int x,y;
	 std::cin >> x >> y;
	 K::Point_2 p(x,y);
	 lamps.push_back(p);
      }

      binSearch();
   }

   void binSearch() {
      std::vector<int> res;
      res.reserve(n);
      {
         Triangulation t;
	 build(t,n);
	 if(hasSurvivors(t,res,true)) {
	    for(int i : res) {
	       std::cout << i << " ";
	    }
	    std::cout << std::endl;
	    return;
	 }
      }
      int lower = 1;
      int upper = n;
      // find last lamp that leaves no p behind.

      while(lower<upper) {
         int m = (lower+upper)/2;
         Triangulation t;
         build(t,m);
	 if(hasSurvivors(t,res,false)) {
	    lower = m+1;
	 } else {
	    upper = m;
	 }
      }

      {
         Triangulation t;
	 build(t,upper);
	 assert(!hasSurvivors(t,res,false));
      }
      {
         Triangulation t;
         build(t,upper-1);
	 hasSurvivors(t,res,true);
	 for(int i : res) {
	    std::cout << i << " ";
	 }
	 std::cout << std::endl;
      }
   }

   void build(Triangulation &t, int nlamps) {
      t.insert(lamps.begin(), lamps.begin()+nlamps);
   }

   bool hasSurvivors(Triangulation &t,std::vector<int> &res, bool doReturn) {
      if(doReturn) {res.clear();}
      for(int i=0; i<m; i++) {
         Participant &part = participants[i];
	 K::Point_2 p(part.x,part.y);

	 const auto &v = t.nearest_vertex(p);
	 if(!t.is_infinite(v)) {
	    if(CGAL::squared_distance(p, v->point()) >= (h+part.r)*(h+part.r)) {
	       if(doReturn) {
	          res.push_back(i);
	       } else {
	          return true;
	       }
	    }
	 }
      }
      return (res.size() > 0);
   }
};

int main() {
   std::ios_base::sync_with_stdio(false);
   
   int t;
   std::cin >> t;
   for(int tt=0; tt<t; tt++) {
      Case c;
   }
}


