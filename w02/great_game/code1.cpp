#include <iostream>
#include <vector>

int main() {
   std::ios_base::sync_with_stdio(false);
   
   std::vector<bool> player = {false,true,false,true};
   std::vector<bool> color = {false,true,true,false};

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
      
      std::vector<std::vector<std::vector<bool>>> table;
      table.resize(4);
      
      for(int s=0; s<4; s++) {
	 table[s].resize(n);
         for(int i=0;i<n;i++) {
            table[s][i].resize(n);
	 }
         for(int i=0;i<n;i++) {
	    table[s][i][n-1] = true;
	    table[s][n-1][i] = false;
	 }
      }

      for(int i=n-2;i>=r-1;i--) {
         for(int j=n-2;j>=b-1;j--) {
	    for(int s=0;s<4;s++) {
	       int nextState = (s+1)%4;
               bool p = player[s];
               bool c = color[s];
	       
	       int res(not p);// assume player cannot win
               
               if(c) {
	          // move j
		  for(auto jj : transition[j]) {
		     if(table[nextState][i][jj]==p) {res = p;break;}
		  }
	       }else{
		  // move i
		  for(auto ii : transition[i]) {
		     if(table[nextState][ii][j]==p) {res = p;break;}
		  }
	       }

	       table[s][i][j] = res;
	    }
	 }
      }
      std::cout << table[0][r-1][b-1] << std::endl;
   }
}
