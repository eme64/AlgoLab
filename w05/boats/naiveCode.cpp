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

const bool EDEBUG = false;


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
   
   int boundary;
   int count;

   void read() {
      std::cin >> n;
      l.resize(n);
      p.resize(n);
      for(int i=0;i<n;i++) {
	 std::cin >> l[i] >> p[i];
      }
      boundary = INT_MIN;
      count = 0;
   };
   
   void run() {
      std::vector<bool> taken(n,false);
      
      for(int i=0;i<n;i++) {
         
	 int minEnd = INT_MAX;
	 int candidate = -1;
         for(int j=0;j<n;j++) {
	    if(not taken[j]) {
	       if(boundary<=p[j]) {
	          //candidate:
		  if(boundary>=p[j]-l[j]) {
		     if(minEnd > boundary+l[j] or (minEnd==boundary+l[j] and candidate>=0 and p[j]<p[candidate])) {
		        minEnd = boundary+l[j];
			candidate = j;
		     }
		  } else {
		     if(minEnd > p[j]) {
		        minEnd = p[j];
			candidate = j;
		     }
		  }
	       }
	    }
	 }

	 if(candidate == -1) {
	    break; // no new candidate found
	 } else {
	    count++;
	    assert(not taken[candidate]);
	    taken[candidate] = true;
	    boundary = minEnd;
	    std::cout << "take " << candidate << " to " << boundary << std::endl;
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
