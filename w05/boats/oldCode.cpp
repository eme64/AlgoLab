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
   int pos;int snd = 0;
   int index;
   Element(int pos,int index) : pos(pos),index(index) {};
   Element(int pos,int snd,int index) : pos(pos),snd(snd),index(index) {};
};
bool operator > (const Element& e1, const Element& e2) {
   return e1.pos > e2.pos or (e1.pos==e2.pos and e1.snd > e2.snd);
};

typedef std::priority_queue<Element,std::vector<Element>,std::greater<Element>> PQ;

struct Test {
   int n;
   std::vector<int> p;
   std::vector<int> l;
   std::vector<bool> taken;
   std::vector<Event> events;
   int boundary;
   PQ activeFree; // contains boat for p if boundary <= p-l
   PQ activeFreeBegin; // contains boat for p-l if boundary <= p-l
   PQ activeLen; // contains boat for l if boundary > p-l
   
   int count;

   const Element nullEl = Element(INT_MAX,-1);

   void read() {
      std::cin >> n;
      events.reserve(2*n);
      l.resize(n);
      p.resize(n);
      taken.resize(n);
      for(int i=0;i<n;i++) {
	 std::cin >> l[i] >> p[i];
         events.push_back(Event(i,true,p[i]-l[i]));
         events.push_back(Event(i,false,p[i]+l[i]));
         taken[i]=false;
      }
      std::sort(events.begin(),events.end());
      boundary = events[0].pos -1;
      count = 0;
   };
   
   void insert(int i) {
      // decide weather in active 1 or 2:
      if(boundary <= p[i]-l[i]) {
         activeFree.push(Element(p[i],l[i],i));
         activeFreeBegin.push(Element(p[i]-l[i],i));
      } else {
         activeLen.push(Element(l[i],p[i],i));
      }
   };
   
   Element peekActiveFree() {
      while(!activeFree.empty()) {
         Element e = activeFree.top();
	 if(p[e.index]-l[e.index] < boundary or taken[e.index]) {
	    activeFree.pop();//boundary has constrained it
	 } else {
	    return e;
	 }
      }
      return nullEl;
   }
 
   Element peekActiveLen() {
      while(!activeLen.empty()) {
	 //std::cout << "size before " << activeLen.size() << std::endl;
	 if(activeLen.size()>2*n) {assert(false);}
	 Element e = activeLen.top();
	 if(activeLen.size()>2*n) {assert(false);}
	 if(p[e.index] < boundary or taken[e.index]) {
	    if(activeLen.size()==0) {assert(false);}
	    activeLen.pop();//boundary has made it impossible
	    if(activeLen.size()>2*n) {assert(false);}
	 } else {
	    return e;
	 }
      }
      return nullEl;
   }
   
   Element peekMin() {
      // see what minimum end value is
      Element e1 = peekActiveFree();
      Element e2 = peekActiveLen();
      //std::cout << "peekMin " << e1.index << " " << e2.index << std::endl;
      if(e1.index==-1 and e2.index==-1) {return nullEl;}
      if(e1.index==-1) {return Element(l[e2.index]+boundary,e2.index);}
      if(e2.index==-1) {return e1;}
      return (e1.pos < l[e2.index] + boundary) ? e1:Element(l[e2.index]+boundary,e2.index);
   }
   
   void popMin() {
      Element e1 = peekActiveFree();
      Element e2 = peekActiveLen();
      if(e1.index==-1 and e2.index==-1) {return;}
      if(e1.index==-1) {activeLen.pop();return;}
      if(e2.index==-1) {activeFree.pop(); return;}
      if (e1.pos < l[e2.index] + boundary) {
         activeFree.pop();
      } else {
         activeLen.pop();
      }
   }
   
   void boundaryIs(int b) {
      boundary = b;

      // process activeFreeBegin -> put into activeLen
      while(!activeFreeBegin.empty()) {
         Element e = activeFreeBegin.top();
         if(boundary>p[e.index]-l[e.index]) {
	    //std::cout << "boundary intersects " << e.index << " " << boundary << std::endl;
	    activeFreeBegin.pop();
	    activeLen.push(Element(l[e.index],p[e.index],e.index));
	 } else {
	    return;
	 }
      }
   }

   void remove(int i) {
      // check if i could still work:
      int top = p[i]+l[i];

      while(true) {
         Element e = peekMin();
         if(e.index==-1) {break;} // take as long as there is sth
         if(e.pos>top) {break;} // take up to p+l
         // take e:
	 assert(taken[e.index]==false);
	 assert(boundary+l[e.index]<=e.pos);
	 assert(e.pos <= top);
         count++;
         popMin(); // pop e
         taken[e.index]=true;
         //std::cout << "take " << e.index << " " << e.pos << " " << l[e.index] << " " << p[e.index] << std::endl;
         boundaryIs(e.pos);
      }
   };

   void run() {
      for(int i=0;i<2*n;i++) {
         const Event &ev(events[i]);
	 if(ev.isBegin) {
	    // handle begin
            //std::cout << "begin " << ev.index << std::endl;
	    insert(ev.index);
	 } else {
	    // handle end
            //std::cout << "end " << ev.index << std::endl;
	    remove(ev.index);
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
