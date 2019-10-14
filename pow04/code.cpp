#include <iostream>
#include <vector>
#include <algorithm>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

// idea really only works in cases 1-3, breaks down as soon as y1 < y0. took me a while to find such an example.

typedef CGAL::Exact_predicates_exact_constructions_kernel K;

typedef CGAL::Line_2<K> L;
typedef CGAL::Point_2<K> P;
typedef CGAL::Ray_2<K> R;

enum State {i1,i2,none};

struct Biker {
   int index;
   K::FT y0, dx, dy, dyAbs, y1;
   Biker(int index, long y0, long x1, long y1) : index(index), y0(y0), dx(x1), y1(y1), dy(y1-y0), dyAbs(std::abs(y1-y0)) {};
};

State check(const Biker& e1, const Biker& e2) {
   if (not CGAL::do_intersect(R(P(0,e1.y0),P(e1.dx,e1.y1)),R(P(0,e2.y0),P(e2.dx,e2.y1)))) {return none;}
   CGAL::Orientation o = CGAL::orientation(P(0,0),P(e1.dx,e1.dyAbs),P(e2.dx,e2.dyAbs));
   if(o==CGAL::LEFT_TURN) {return i2;}
   if(o==CGAL::RIGHT_TURN) {return i1;}
   if(e1.dy>0) {return i2;} else {return i1;}
}

bool operator < (const Biker& e1, const Biker& e2) {
     return e1.y0 < e2.y0;
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
      std::vector<int> next(n+2);
      std::vector<int> prev(n+2);
      next[0] = 0; next[n] = n; // sentinel entries
      prev[0] = -1; prev[n] = n-1;
      for(int i=0; i<n; i++) {
         long y0(0); long x1(0); long y1(0);
         std::cin >> y0 >> x1 >> y1;
         bikers.push_back(Biker(i,y0,x1,y1));
         next[i+1] = i+1;
         prev[i+1] = i-1;
      }
      
      std::sort(bikers.begin(),bikers.end());
      
      int nDel = 1;
      while(nDel > 0) {
         nDel = 0;
         //std::cout << "round" << std::endl;
         int i = next[0]; // read sentinel
         while(i<n) {
            int nexti = next[i+1]; // next of i
            if(nexti < n) {
               State s = check(bikers[i],bikers[nexti]);
               if(s==i1) {
                  // must delete i, move i one back:
                  nDel++;
                  //std::cout << "del i: " << i << " " << nexti << std::endl;
                  int lasti = prev[i+1];
                  next[lasti+1] = nexti;
                  prev[nexti+1] = lasti;
                  nexti = (lasti>=0)?lasti:nexti;
               } else if(s==i2) {
                  // must delete nexti:
                  nDel++;
                  //std::cout << "del nexti: " << i << " " << nexti << std::endl;
                  int afternexti = next[nexti+1];
                  next[i+1] = afternexti;
                  prev[afternexti+1] = i;
                  nexti = i;
               }
            }
            
            i = nexti;
         }
      }
      
      
      std::vector<int> winners;
      winners.reserve(n);
      
      int i=next[0];
      while(i<n) {
         winners.push_back(bikers[i].index);
         i = next[i+1];
      } 
      std::sort(winners.begin(),winners.end());
      for(i=0;i<winners.size();i++) {
         std::cout << winners[i] << " ";
      }
      std::cout << std::endl;
   }
}



