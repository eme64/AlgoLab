// Idea:

#include <iostream>


int main() {
   std::ios_base::sync_with_stdio(false);
   
   int t(0);
   std::cin >> t;

   for(int i = 0; i<t; i++) {
      int n(0);
      std::cin >> n;
      
      int sum(0);
      
      for(int j = 0; j<n; j++) {
         int x(0);
         std::cin >> x;
         
         sum+= x;
      }
      std::cout << sum << std::endl;
   }
}
