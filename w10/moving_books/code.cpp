#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

struct Case {
   int n,m;
   std::vector<int> strength;
   std::vector<int> boxes;
   bool possible = true;
   Case() {
      std::cin >> n >> m;
      strength.reserve(n);
      for(int i=0; i<n; i++) {
         int s;
	 std::cin >> s;
	 strength.push_back(s);
      }
      std::sort(strength.begin(),strength.end());

      boxes.resize(n,0);
      for(int i=0; i<m; i++) {
         int w;
	 std::cin >> w;
	 insert(w);
      }
      //for(int i=0; i<n; i++) {
      //   std::cout << boxes[i] << " ";
      //}
      //std::cout << std::endl;
      
      if(possible) {
	 assert(!check(0));
	 assert(check(m));
         std::cout << 3*binSearch()-1 << std::endl;
      } else {
         std::cout << "impossible" << std::endl;
      }
   };

   inline void insert(int w) {
      if(w > strength[n-1]) {possible = false; return;}
      if(w<=strength[0]) {boxes[0]++; return;}
      int l = 1;
      int u = n-1;
      // find first that can hold it
      while(l<u) {
         int m = (l+u)/2;
	 if(strength[m]>=w) {
	    u = m;
	 } else {
            l = m+1;
	 }
      }
      boxes[u]++;
   }

   inline bool check(int r) {
      int pending = 0;
      for(int i=0; i<n; i++) {
         pending+=boxes[i];
	 int take = std::min(pending, r);
	 pending-=take;
      }
      return (pending==0);
   }

   int binSearch() {
      int l = 1;
      int u = m;

      while(l<u) {
         int m=(l+u)/2;
         if(check(m)) {
	    u = m;
	 } else {
            l = m+1;
	 }
      }
      assert(check(u));
      assert(!check(u-1));
      return u;
   }
};

int main() {
   std::ios_base::sync_with_stdio(false);

   int t;
   std::cin >> t;
   for(int tt=0; tt<t; tt++) {Case c;}
}



