// idea:
// find segment with at most 10 jedi (find all end+1) -> count
//  - first time circle: warm up counter
//  - second time: mark down counts -> keep min
// O(n log n)
// for each of the jedi there run algo:
// pick jedi: run in circle, choose min that works
// O(n log n)

#include <cassert>
#include <iostream>
#include <vector>
#include <algorithm>


enum Kind : char {
   Begin = 0,
   Check = 1,
   End = 2,
};

struct Element {
   int position;
   Kind kind;
   Element(int index, int position, Kind kind) : position(position), kind(kind) {};
};

bool operator < (const Element& e1, const Element& e2) {
     return e1.position < e2.position or (e1.position == e2.position and e1.kind < e2.kind);
}

struct Segment {
   int begin;
   int end;
   Segment(int begin, int end) : begin(begin), end(end) {};
};

bool operator < (const Segment& e1, const Segment& e2) {
     return e1.end < e2.end;
}

int greedy(std::vector<Segment> &segments, int lower, int upper) {
   int cnt = 0;
   for(Segment &s : segments) {
      if(lower <= s.begin && s.end <= upper) {
         cnt++;
	 lower = s.end+1;
      }
   }
   return cnt;
}

int main() {
   std::ios_base::sync_with_stdio(false);

   int t(0);
   std::cin >> t;
   for(int tt=0; tt<t; tt++) {
      int n(0), m(0);
      std::cin >> n >> m;
      
      std::vector<Element> elements;
      elements.reserve(4*n);
      std::vector<int> begin(n);
      std::vector<int> end(n);
      
      int numActive(0);
      for(int i=0;i<n;i++) {
         std::cin >> begin[i] >> end[i];
	 begin[i] = begin[i]%m;
	 end[i] = end[i]%m;
         elements.push_back(Element(i,(begin[i]-1+m)%m,Check));
         elements.push_back(Element(i,begin[i]%m,Begin));
         elements.push_back(Element(i,begin[i]%m,Check));
         elements.push_back(Element(i,end[i]%m,End));
         if(begin[i]>end[i]) {
	     numActive++;
	 }
      }
      std::sort(elements.begin(),elements.end());
     
      int minPos = -1;
      int minActive = n+1;

      for(Element &e : elements) {
         if(e.kind == Begin) {
	    numActive++;
	 } else if(e.kind == End) {
	    numActive--;
	 } else {
            if(minActive > numActive) {
	       minActive = numActive;
	       minPos = e.position;
	    }
	 }
      }
      
      //std::cout << minActive << " " << minPos << std::endl;
      
      // now input segments, sorted by end position:
      std::vector<int> anchors;
      std::vector<Segment> segments;
      segments.reserve(n);
      for(int i=0;i<n;i++) {
         int a = (begin[i]+m-minPos)%m;
         int b = (end[i]+m-minPos)%m;
	 begin[i] = a;
	 end[i] = b;
	 if(b<a or a==0) {
	    //std::cout << "active: " << i << std::endl;
	    anchors.push_back(i);
	 } else {
            segments.push_back(Segment(a,b));
	 }
      }
      std::sort(segments.begin(),segments.end());
      
      int cnt = 0;
      cnt = greedy(segments, 0, m-1);
      
      for(int &i : anchors) {
         cnt = std::max(cnt, greedy(segments, end[i]+1, begin[i]-1)+1);
      }
      
      std::cout << cnt << std::endl;
   }
}






