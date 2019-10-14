#include <iostream>
#include <vector>
#include <algorithm>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

// idea really only works in cases 1-3, breaks down as soon as y1 < y0. took me a while to find such an example.

typedef CGAL::Exact_predicates_exact_constructions_kernel K;

typedef CGAL::Line_2<K> L;
typedef CGAL::Point_2<K> P;

struct Biker {
   int index;
   K::FT y0, dx, dy, dyAbs;
   Biker(int index, long y0, long x1, long y1) : index(index), y0(y0), dx(x1), dy(y1-y0), dyAbs(std::abs(y1-y0)) {};
};

bool operator < (const Biker& e1, const Biker& e2) {
   CGAL::Orientation o = CGAL::orientation(P(0,0),P(e1.dx,e1.dyAbs),P(e2.dx,e2.dyAbs));
   return (o==CGAL::LEFT_TURN)
       or (o==CGAL::COLLINEAR
           and (CGAL::orientation(P(0,0),P(e1.dx,e1.dy),P(e2.dx,e2.dy))==CGAL::LEFT_TURN
                or (e1.dyAbs >0) ? (e1.y0 < e2.y0) : (e1.y0 > e2.y0)));
//   return (e1.dyAbs/e1.dx < e2.dyAbs/e2.dx)
//       or (e1.dyAbs/e1.dx == e2.dyAbs/e2.dx and (e1.dy/e1.dx < e2.dy/e2.dx or 
//                                                 (e1.dyAbs >0) ? (e1.y0 < e2.y0) : (e1.y0 > e2.y0)));
}

int main() {
   std::ios_base::sync_with_stdio(false);

   int t(0);
   std::cin >> t;
   for(int tt=0; tt<t; tt++) {
      int n(0);
      std::cin >> n;
      
      std::vector<Biker> bikers;
      bikers.reserve(n);
      
      for(int i=0; i<n; i++) {
         long y0(0); long x1(0); long y1(0);
         std::cin >> y0 >> x1 >> y1;
         bikers.push_back(Biker(i,y0,x1,y1));
      }
      
      std::sort(bikers.begin(),bikers.end());
      
      std::vector<int> winners;
      winners.reserve(n);      
      
      Biker relTop = bikers[0];
      Biker relBottom = bikers[0];
      winners.push_back(relTop.index);
      std::cout << "first " << relTop.index << std::endl;
      
      for(int j=1;j<n;j++) {
         Biker curr = bikers[j];
         std::cout << "curr: " << curr.index << " " << CGAL::to_double(curr.dy/curr.dx) << std::endl;
         if(curr.dy==0) {
            // middle flat cases
            winners.push_back(curr.index);
            if(relTop.y0 < curr.y0) {relTop = curr;}
            if(relBottom.y0 > curr.y0) {relBottom = curr;}
            std::cout << "center " << curr.index << std::endl;
         } if(curr.dy>0) {
            // top cases
            if(curr.y0 > relTop.y0) {
               winners.push_back(curr.index);
               relTop = curr;
               std::cout << "top " << curr.index << std::endl;
            }
         } else {
            // bottom cases
            if(curr.y0 < relBottom.y0) {
               winners.push_back(curr.index);
               relBottom = curr;
               std::cout << "bottom " << curr.index << std::endl;
            }
         }
      }
      
      std::sort(winners.begin(),winners.end());
      for(int w=0; w<winners.size(); w++) {std::cout << winners[w] << " ";}
      std::cout << std::endl;
   }
}



