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

struct Element {
   int pos;
   int index;
   Element(int pos,int index) : pos(pos),index(index) {};
};
bool operator < (const Element& e1, const Element& e2) {
   return e1.pos < e2.pos;
};
struct Test {
   int n;
   std::vector<int> p;
   std::vector<int> l;
   std::vector<bool> marked;
   std::vector<Event> events;
   int boundary;
   std::priority_queue<Element> active1; // contains boat for p if boundary <= p-l
   std::priority_queue<Element> contactList; // contains boat for p-l if boundary <= p-l
   std::priority_queue<Element> active2; // contains boat for l if boundary > p-l
   
   int count;

   const Element nullEl(INT_MAX,-1);

   void read() {
      std::cin >> n;
      events.reserve(2*n);
      l.resize(n);
      p.resize(n);
      marked.resize(n);
      for(int i=0;i<n;i++) {
	 std::cin >> l[i] >> p[i];
         events.push_back(Event(i,true,p[i]-l[i]));
         events.push_back(Event(i,false,p[i]+l[i]));
	 marked[i] = false;
      }
      std::sort(events.begin(),events.end());
      boundary = events[0].pos -1;
      count = 0;
   };
   
   void insert(int i) {
      // decide weather in active 1 or 2:
      if(boundary <= p[i]-l[i]) {
         active1.push(Element(p[i],i));
         contactList.push(Element(p[i]-l[i],i));
      } else {
         active2.push(Element(l[i],i));
      }
   };
   
   Element& peekMin(std::priority_queue<Element> &q) {
      // peekMin, but remove marked Elements
      while(q.top()!=q.end()) {
         Element &e = q.top();
	 if(marked[e.index]) {
	    q.pop();
	 } else {
	    return e;
	 }
      }
      return nullEl;
   }

   int peekMin() {
      // see what minimum end value is
      int minEnd(INT_MAX);
      Element &e1 = peekMin(active1);
      if(e1!=nullEl) {
         minEnd = e1.pos;
      }
      Element &e2 = peekMin(active2);
      
   } 

   void remove(int i) {
      // check if i could still work:
      if(boundary <= p[i]) {
         // could take i, or something better
         // take as much up to p+l
	 int top = p[i]+l[i];

	 while() {}
	 //std::cout << "take " << i << std::endl;
	 //int boundaryI = std::max(boundary + l[i], p[i]);
	 //count++;
      } else {
         // cannot take i
	 std::cout << "reject " << i << std::endl;
      }
      marked[i] = true;
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
	    remove(i);
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
      std::cout << test.count << std::endl;
   }
}
