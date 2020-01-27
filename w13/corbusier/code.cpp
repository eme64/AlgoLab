#include <iostream>
#include <vector>

struct Test {
   int n, i, k;
   Test() {
      std::cin >> n >> i >> k;
      
      std::vector<bool> T(k,false);
      std::vector<bool> Tnew(k,false);

      for(int j=0; j<n; j++) {
         int h;
	 std::cin >> h;
	 h = h % k;
	 //std::cout << h << std::endl;
	 for(int a=0; a<k; a++) {
	    Tnew[a] = T[a] or T[(a+k-h) % k];
	    //std::cout << Tnew[a];
	 }
	 Tnew[h] = true;
	 //std::cout << std::endl;
	 std::swap(T,Tnew);
      }
      
      if(T[i % k]) {
         std::cout << "yes" << std::endl;
      } else {
         std::cout << "no" << std::endl;
      }
   }
};

int main() {
   std::ios_base::sync_with_stdio(false);
   
   int t;
   std::cin >> t;
   for(int i=0; i<t; i++) {Test tt;}
}
