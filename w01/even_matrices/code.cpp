// Idea 1:
// calculate prefix sum O(n2)
// for each tuple (i1,i2,j1,j2):
//    calculate sum from prefix sum:
//    p(i2,j2) - p(i1-1,j2) - p(i2,j1-1) + p(i1-1,j1-1)
// O(n4)

// Idea 2:
// combine 2 prefix rows: add, which pairs are even?
// n2 row combinations, n per combination



#include <iostream>
#include <vector>

int main() {
   std::ios_base::sync_with_stdio(false);
   
   int t(0);
   std::cin >> t;

   for(int ti = 0; ti<t; ti++) {
      int n(0);
      std::cin >> n;
      
      int tupels(0);
      std::vector<std::vector<bool>> prefix;
      
      prefix.resize(n+1);
      prefix[0].resize(n+1);
      for(int i = 1; i<n+1; i++){
         prefix[i].resize(n+1);
         for(int j = 1; j<n+1; j++){
            int x(0);
            std::cin >> x;
            prefix[i][j] = prefix[i-1][j] xor prefix[i][j-1] xor prefix[i-1][j-1] xor (x==1);
         }
      
         for(int i2 = 0; i2<i; i2++) {
            int E(0);
            bool sum(false);
            for(int j = 0; j<n+1; j++) {
               sum = prefix[i][j] xor prefix[i2][j];
               E += sum?0:1;
            }
            int O(n+1-E);
            tupels += E*(E-1)/2 + O*(O-1)/2;
            //std::cout << i << " " << i2 << " " << E << " " << O << " " << tupels << std::endl;
         }
      }
      std::cout << tupels << std::endl;
   }
}
