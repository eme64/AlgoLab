// Idea:
// calculate prefix sum
// check which are even E, which odd O
// 
// number even pairs:
// E choose 2, O choose 2, E

#include <iostream>


int main() {
   std::ios_base::sync_with_stdio(false);
   
   int t(0);
   std::cin >> t;

   for(int i = 0; i<t; i++) {
      int n(0);
      std::cin >> n;
      
      int sum(0);
      int E(0);
      for(int j = 0; j<n; j++) {
         int x(0);
         std::cin >> x;
         
         sum += x;
         E += (sum % 2 == 0)?1:0;
      }
      int O(n-E);
      int evenPairs( E + E*(E-1)/2 + O*(O-1)/2 );
      std::cout << evenPairs << std::endl;
   }
}
