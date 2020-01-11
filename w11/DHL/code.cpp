#include <iostream>
#include <vector>


struct Test {
   int n;
   std::vector<long> a;
   std::vector<long> b;
   Test() {
      a.resize(n,0);
      b.resize(n,0);

      for(int i=0; i<n; i++) {std::cin >> a[i];}
      for(int i=0; i<n; i++) {std::cin >> b[i];}
      
      // prefix sum: easily get cost for one side.
      // n4: table, for x,y look for cheapest extension option O(n2)
      // basically enumerates all possible loads


      std::cout << 0 << std::endl;
   }
};


int main() {
   std::ios_base::sync_with_stdio(false);
   
   int t;
   std::cin >> t;
   for(int tt=0; tt<t; tt++) {
      Test ttt;
   }
}



