#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <climits>
#include <queue>
#include <cassert>
#define NDEBUG

// greedy: pick with current first end
// this number is not stable: can be pushed forward
// many could be updated

// DP:
// for #boats keep maxX -> O(n2) -> n too large
// might as well pick min end until none left O(n2)


// scanline:
// enter: begin(p-l), end(p+l)
// on begin: put in active set
// on end: take me or better one that could end earlier
// take one that ends earliest, up to end of this
// ends earliest:
//    activeFree: if not constrained by boundary (b <=p-l), enter for p
//    activeLen: else - enter for l
// pick min of the two heaps
// when one picked: update boundary
// find all that are now constrained: need activeFreeBegin minheap
// should lead to O(nlogn)

// better: similar to scanline, but without min-heap, rather case distinction and min line.
// experiment

struct Event {
   int pos; int len;
   int index;
   Event(int index,int pos, int len) :pos(pos),len(len),index(index) {};
};
bool operator < (const Event& e1, const Event& e2) {
   return (e1.pos < e2.pos) or (e1.pos == e2.pos and e1.len < e2.len);
};

struct Test {
   int n;
   std::vector<int> p;
   std::vector<int> l;
   std::vector<Event> events;
   
   int boundary;
   int rightMin;
   int currentIndex = -1;
   int count;

   void read() {
      std::cin >> n;
      events.reserve(n);
      l.resize(n);
      p.resize(n);
      for(int i=0;i<n;i++) {
	 std::cin >> l[i] >> p[i];
         events.push_back(Event(i,p[i],l[i]));
      }
      std::sort(events.begin(),events.end());
      boundary = events[0].pos -1;
      rightMin = boundary;
      count = 0;
   };
   
   void run() {
      for(int i=0;i<n;i++) {
         const Event &ev(events[i]);
         
         if(ev.pos - ev.len >= boundary) {
            // unconstrained by boundary
            if(currentIndex>=0 and ev.pos >= rightMin) {
               // over min line: take prev, then see what do about this
               
            } else {
               // before min line/no min line: must take this
               count++;
               currentIndex = -1; // kill previous ones
               boundary = ev.pos;
               rightMin = boundary;
               std::cout << "take (free) " << ev.index << " from " << (ev.pos-ev.len) << " to " << ev.pos << std::endl;
            }
         } else {
            // constrained
         }
      }
   };
};

int main () {
   std::ios_base::sync_with_stdio(false);
   
   int t(0);
   std::cin >> t;
   
   Test *test = new Test();

   for(int tt=0; tt<t; tt++) {
      test->read();
      test->run();
      std::cout << test->count << std::endl;
   }
}
