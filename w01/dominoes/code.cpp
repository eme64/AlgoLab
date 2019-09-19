// Idea:

#include <iostream>


int main() {
   std::ios_base::sync_with_stdio(false);
   
   int t(0);
   std::cin >> t;

   for(int i = 0; i<t; i++) {
      int n(0);
      std::cin >> n;
      
      int reach(2);
      int counter(0);

      while(reach>1 && counter<n) {
         int x(0);
         std::cin >> x;
         
         counter++;
         reach = (reach-1<x)?x:reach-1;
      }
      
      std::cout << counter << std::endl;
      
      while(counter<n){
         int x(0);
         std::cin >> x;
         counter++;
      }
   }
}
