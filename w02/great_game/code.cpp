// runtime seems ok, but wrong answer
#include <iostream>
#include <vector>
#include <functional>

int main() {
   std::ios_base::sync_with_stdio(false);
   

   int t(0);
   std::cin >> t;

   for(int tt=0;tt<t;tt++) {
      int n(0), m(0), r(0), b(0);
      std::cin >> n >> m >> r >> b;
      std::vector<std::vector<int>> transition;
      transition.resize(n);
      for(int mm=0; mm<m; mm++) {
         int u(0),v(0);
	 std::cin >> u >> v;
         transition[u-1].push_back(v-1);
      }
      
      std::vector<int> dMin, dMax;
      dMin.resize(n);
      dMax.resize(n);
      dMin[n-1] = 0;
      dMax[n-1] = 0;

      for(int i=n-2;i>=0;i--) {
         int dMinNew = n;
         int dMaxNew = 0;
         for(auto j : transition[i]) {
            dMinNew = std::min(dMinNew, dMax[j]) + 1;
            dMaxNew = std::max(dMaxNew, dMin[j]) + 1;
         }
         dMin[i] = dMinNew;
         dMax[i] = dMaxNew;
      }
      
      std::cout << ((dMin[r-1]>dMin[b-1])?1:0) << std::endl;
   }
}
