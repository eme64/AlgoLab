#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <climits>
#include <queue>

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
// once in life go from active1 to active2
// active1: active but maxBoundary not contacted - sort by p
//  - also: contact list: sort by p-l - remove if overstepped
// active2: contact with maxBoundary - sort by l
// should lead to O(nlogn)


struct Event {
   bool isBegin;
   int pos;
   int index;
   Event(int index,bool isBegin,int pos) :isBegin(isBegin),pos(pos),index(index) {};
};
bool operator < (const Event& e1, const Event& e2) {
   return (e1.pos < e2.pos) or (e1.pos == e2.pos and e1.isBegin==false and e2.isBegin==true);
};

struct Test {
   int n;
   std::vector<int> p;
   std::vector<int> l;
   std::vector<Event> events;
   int boundary;
   
   void read() {
      std::cin >> n;
      events.reserve(2*n);
      l.resize(n);
      p.resize(n);
      for(int i=0;i<n;i++) {
	 std::cin >> l[i] >> p[i];
         events.push_back(Event(i,true,p[i]-l[i]));
         events.push_back(Event(i,false,p[i]+l[i]));
      }
      std::sort(events.begin(),events.end());
      boundary = events[0].pos -1;
   };
   
   void insert(int i) {
   };

   void run() {
      for(int i=0;i<2*n;i++) {
         const Event &ev(events[i]);
	 if(ev.isBegin) {
	    // handle begin
            std::cout << "begin " << ev.index << std::endl;
	    insert(i);
	 } else {
	    // handle end
            std::cout << "end " << ev.index << std::endl;
	 }
      }
   };
};

int main () {
   std::ios_base::sync_with_stdio(false);
   
   int t(0);
   std::cin >> t;
   
   Test test;

   for(int tt=0; tt<t; tt++) {
      test.read();
      test.run();
   }
}
