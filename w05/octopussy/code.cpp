#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <climits>
#include <queue>

typedef std::priority_queue<int,std::vector<int>,std::greater<int>> PQ;

int main () {
   std::ios_base::sync_with_stdio(false);
   
   int t(0);
   std::cin >> t;
   
   for(int tt=0; tt<t; tt++) {
      int n(0);
      std::cin >> n;
      
      std::vector<int> table(n);
      for(int i=0;i<n;i++) {
         std::cin >> table[i];
      }
      
      for(int i=0; i<=(n-3)/2;i++) {
         table[2*i+1] = std::min(table[2*i+1],table[i]-1);
         table[2*i+2] = std::min(table[2*i+2],table[i]-1);
      }

      PQ q;

      for(int i=0;i<n;i++) {
         q.push(table[i]);
      }
      bool fail=false;
      for(int i=0;i<n;i++) {
         int t = q.top();
	 q.pop();
	 if(i+1>t) {
	    fail = true;
	 }
      }
      
      
      std::cout << (fail?"no":"yes") << std::endl;
   }
}
