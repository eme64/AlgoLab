#include <iostream>
#include <vector>
#include <functional>
enum Entry : char {H=0,M=1,empty=2};
const std::vector<bool> player = {false,true,false,true};
const std::vector<bool> color = {false,true,true,false};

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
      
      std::vector<std::vector<std::vector<Entry>>> table;
      table.resize(4);
      
      for(int s=0; s<4; s++) {
	 table[s].resize(n);
         for(int i=0;i<n;i++) {
            table[s][i].resize(n,Entry::empty);
	 }
         for(int i=0;i<n;i++) {
	    table[s][i][n-1] = Entry::M;
	    table[s][n-1][i] = Entry::H;
	 }
      }
      
      std::function<Entry(int,int,int)> f;
      f = [&table,&transition,&f](int s,int i,int j)->Entry{
         if(table[s][i][j]==Entry::empty) {
 	    const int nextState = (s+1)%4;
            const bool p = player[s];
            const bool c = color[s];
	    
            if(c) {
	       // move j
	       for(auto jj : transition[j]) {
	          if(f(nextState,i,jj)==p) {
	             table[s][i][j] = p? Entry::M : Entry::H;
		     return table[s][i][j];
		  }
	       }
	    }else{
	       // move i
	       for(auto ii : transition[i]) {
	          if(f(nextState,ii,j)==p) {
	             table[s][i][j] = p? Entry::M : Entry::H;
		     return table[s][i][j];
	          }
	       }
	    }

	    table[s][i][j] = (not p)? Entry::M : Entry::H;
	 }
      	 return table[s][i][j];
      };
      
      std::cout << f(0,r-1,b-1) << std::endl;
   }
}
