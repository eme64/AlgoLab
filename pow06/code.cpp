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
   End = 2,
   Check = 1,
};

struct Element {
   int index;
   int position;
   Kind kind;
   Element(int index, int position, Kind kind) : index(index), position(position), kind(kind) {};
};

bool operator < (const Element& e1, const Element& e2) {
     return e1.position < e2.position or (e1.position == e2.position and e1.kind < e2.kind);
}

bool overlap(int a, int b, int c, int d) {
   if(a<=b) {
      if((a<=c and c<=b) or (a<=d and d<=b)) {return true;} // interlocked
      if(c<a and b<d) {return true;} // overbridged
      return false;
   } else {
      if((c>=a or c<=b) or (d>=a or d<=b)) {return true;} // interlocked
      if(d<c) {return true;} // overbridged
      return false;
   }
}


void greedy(int n, int m, int start, int finish, std::vector<Element> &elements, int &cnt, std::vector<int> begin) {
   // find start/finish:
   
   int boundary = start;
   for(auto e : elements) {
      if(e.kind == End and e.position > start) {
         int a = begin[e.index]%m;
         int b = e.position;
         if(not overlap(a,b,finish,boundary)) {
            // take:
            cnt++;
            boundary = b;
         }
      }
   }
   for(auto e : elements) {
      if(e.kind == End and e.position < finish) {
         int a = begin[e.index]%m;
         int b = e.position;
         if(not overlap(a,b,finish,boundary)) {
            // take:
            cnt++;
            boundary = b;
         }
      }
   }
   
   
}

int main() {
   std::ios_base::sync_with_stdio(false);

   assert(not overlap(1,1,2,2));
   assert(not overlap(3,3,2,2));
   assert(not overlap(3,1,2,2));
   assert(    overlap(1,0,2,2));
   assert(    overlap(1,2,2,2));
   assert(    overlap(2,3,2,2));
   assert(    overlap(5,2,2,2));
   assert(    overlap(5,2,2,4));
   
   assert(not overlap(1,3,4,6));
   assert(    overlap(1,5,4,6));
   assert(    overlap(1,7,4,6));
   assert(    overlap(1,0,4,6));
   
   int t(0);
   std::cin >> t;
   for(int tt=0; tt<t; tt++) {
      int n(0), m(0);
      std::cin >> n >> m;
      
      std::vector<Element> elements;
      elements.reserve(3*n);
      std::vector<int> begin(n);
      std::vector<int> end(n);
      
      for(int i=0;i<n;i++) {
         std::cin >> begin[i] >> end[i];
         elements.push_back(Element(i,begin[i]%m,Begin));
         elements.push_back(Element(i,end[i]%m,End));
         elements.push_back(Element(i,(end[i]+1)%m,Check));
      }
      std::sort(elements.begin(),elements.end());
      
      int numActive(0);
      std::vector<bool> active(n,false);
      // first circle scan: warmup:
      for(auto e : elements) {
         if(e.kind == Begin) {
            active[e.index] = true;
            numActive++;
         } else if(e.kind == End) {
            if(active[e.index]==true) {numActive--;}
            active[e.index] = false;
         } else {
            // Check: not yet
         }
      }
      
      std::vector<int> startSet;
      startSet.reserve(10);
      int startPosition(-1);
      // second round: find minimum
      for(auto e : elements) {
         if(e.kind == Begin) {
            assert(active[e.index]==false);
            active[e.index] = true;
            numActive++;
         } else if(e.kind == End) {
            assert(active[e.index]==true);
            active[e.index] = false;
            numActive--;
         } else {
            // Check:
            if(numActive <= 10) {
               if(startSet.size()==0) {
                  startPosition = e.position;
                  for(int i=0;i<n;i++) {
                     if(active[i]) {startSet.push_back(i);}
                  }
               }
            }
         }
      }
      assert(startPosition != -1);
      
      if(startSet.size()==0) {
         int cnt = 0;
         int start = startPosition;
         int finish = startPosition;
         greedy(n,m,start,finish,elements,cnt,begin);
         std::cout << cnt << std::endl;
      } else {
         int maxCnt(0);
         for(int i=0;i<startSet.size(); i++) {
            int cnt = 1;
            int start = end[startSet[i]]%m;
            int finish = begin[startSet[i]]%m;
            greedy(n,m,start,finish,elements,cnt,begin);
            maxCnt = std::max(maxCnt,cnt);
         }
         std::cout << maxCnt << std::endl;
      }
      
      std::cout << startSet.size() << " " << startPosition << std::endl;
   }
}






