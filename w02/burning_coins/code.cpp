#include <iostream>
#include <vector>

int main() {
   std::ios_base::sync_with_stdio(false);
   
   int t(0);
   std::cin >> t;
   for(int tt=0; tt<t; tt++){
      int n(0);
      std::cin >> n;
      
      std::vector<int> v;
      v.resize(n+1);
      std::vector<int> p;
      p.resize(n+1);
      
      int sum(0);
      p[0] = 0;
      
      for(int i=1;i<n+1;i++){
         std::cin >> v[i];
         sum+=v[i];
         p[i]=sum;
      }
      
      for(int l=2; l<=n; l++) {
         for(int i=1; i<=n-l+1; i++) {
            int t = p[i+l-1] - p[i-1] - std::min(v[i],v[i+1]);
            v[i]=t;
            //std::cout << t << " ";
         }
         //std::cout << std::endl;
      }
      std::cout << v[1] << std::endl;
   }
}
