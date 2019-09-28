// Idea: player 0=Holmes wants to be fast on red, slow on black
// player 1=Moriarty wants to be fast on black, slow on red
// play games in parallel (Min/Max)
// at the end count what side ends faster

#include <iostream>
#include <vector>
#include <functional>

const std::vector<bool> color = {false,true,true,false};

int main() {
   std::ios_base::sync_with_stdio(false);
   

   int t(0);
   std::cin >> t;

   for(int tt=0;tt<t;tt++) {
      // read in graph data
      int n(0), m(0), r(0), b(0);
      std::cin >> n >> m >> r >> b;
      std::vector<std::vector<int>> transition;
      transition.resize(n);
      for(int mm=0; mm<m; mm++) {
         int u(0),v(0);
	 std::cin >> u >> v;
         transition[u-1].push_back(v-1);
      }
      
      // start Min/Max calculation
      std::vector<int> dMin, dMax;
      dMin.resize(n);
      dMax.resize(n);
      dMin[n-1] = 0;
      dMax[n-1] = 0;

      for(int i=n-2;i>=0;i--) {
         int dMinNew = 2*n;
         int dMaxNew = -2*n;
         for(auto j : transition[i]) {
            dMinNew = std::min(dMinNew, dMax[j]+1);
            dMaxNew = std::max(dMaxNew, dMin[j]+1);
         }
         dMin[i] = dMinNew;
         dMax[i] = dMaxNew;
      }
      
      // given calculate who is actually faster (respecting order of turns):
      int res0(dMin[r-1]);
      int res1(dMin[b-1]);
      
      int round(0);
      while(res0>0 and res1>0) {
         if(color[round]) {res1--;} else {res0--;}
         round = (round + 1) % 4;
      }
      
      std::cout << (res0==0?0:1) << std::endl;
   }
}
