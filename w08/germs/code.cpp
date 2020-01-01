#include <iostream>
#include <map>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Finite_faces_iterator Face_iterator;

long dtot(K::FT d) {
   // take root, divide by 2
   // subtract 1/2, square
   // round up
   K::FT t2 = CGAL::sqrt(d)*0.5-0.5;
   K::FT t = CGAL::sqrt(t2);
   long apro = CGAL::to_double(t);
   
   while(true) {
      K::FT ro = apro*apro*2+1;
      K::FT a = ro*ro;
      if(a>d) {apro--;}else{break;}
   }
   while(true) {
      K::FT ro = apro*apro*2+1;
      K::FT a = ro*ro;
      if(a<d) {apro++;}else{break;}
   }

   return apro;
}

int main() {
   std::ios_base::sync_with_stdio(false);
   
   while(true) {
      int n;
      std::cin >> n;
      if(n==0) {break;}

      int l,b,r,t;
      std::cin >> l >> b >> r >> t;

      std::vector<K::Point_2> pts;
      pts.reserve(n+4);
      std::map<K::Point_2,int> ptoi;
      std::vector<K::FT> mind;
      mind.reserve(n+4);
      for(int i=0; i<n; i++) {
         int x,y;
	 std::cin >> x >> y;
	 K::Point_2 p(x, y);
         pts.push_back(p);
	 ptoi[p] = i;

	 K::FT d = 2*std::min(x-l, std::min(r-x, std::min(y-b, t-y)));
	 mind.push_back(d*d);
      }
      long big = 1 << 25;
      {
         K::Point_2 p(l-big,b-big);
	 pts.push_back(p);
	 ptoi[p] = n;
	 mind.push_back(0);
      }
      {
         K::Point_2 p(l-big,t+big);
	 pts.push_back(p);
	 ptoi[p] = n+1;
	 mind.push_back(0);
      }
      {
         K::Point_2 p(r+big,b-big);
	 pts.push_back(p);
	 ptoi[p] = n+2;
	 mind.push_back(0);
      }
      {
         K::Point_2 p(r+big,t+big);
	 pts.push_back(p);
	 ptoi[p] = n+3;
	 mind.push_back(0);
      }


      Triangulation tt;
      tt.insert(pts.begin(), pts.end());

      for (Face_iterator f = tt.finite_faces_begin(); f != tt.finite_faces_end(); ++f) {
         K::Point_2 &p0 = f->vertex(0)->point();
         K::Point_2 &p1 = f->vertex(1)->point();
         K::Point_2 &p2 = f->vertex(2)->point();
         
	 int i0 = ptoi[p0];
	 int i1 = ptoi[p1];
	 int i2 = ptoi[p2];

	 K::FT d0 = CGAL::squared_distance(p0,p1);
	 K::FT d1 = CGAL::squared_distance(p1,p2);
	 K::FT d2 = CGAL::squared_distance(p2,p0);

	 mind[i0] = std::min(mind[i0], std::min(d0,d2));
	 mind[i1] = std::min(mind[i1], std::min(d0,d1));
	 mind[i2] = std::min(mind[i2], std::min(d1,d2));
      }

      std::sort(mind.begin(), mind.end()-4);

      std::cout << dtot(mind[0]) << " "
	        << dtot(mind[n/2]) << " "
		<< dtot(mind[n-1]) << std::endl;
   }
}
