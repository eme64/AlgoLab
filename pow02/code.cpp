#include <iostream>
#include <vector>
#include <climits>

// T(i,l) = "your turn now, with subarray starting at i with length l"
// T(i,l) = max( pick first or last , then minimum of other players options)

int main() {
   std::ios_base::sync_with_stdio(false);

   int t(0);
   std::cin >> t;

   for(int tt=0;tt<t;tt++) {
      int n(0),m(0),k(0);
      std::cin >> n >> m >> k;
      
      std::vector<int> x;
      x.resize(n);
      
      for(int i=0; i<n; i++) {
         std::cin >> x[i];
      }
      
      if(n<=k) {
         std::cout << 0 << std::endl;
	 continue;
      }
      // assume n > k
      
      int beforeTurns(k);
      int numTurns((n-k)/m);
      int afterTurns(n-k-m*numTurns);
      if(numTurns>0) {afterTurns-=m-1;}

      // calculate base (last turn):
      std::vector<int> table;
      table.resize(n);
      int turnMin = INT_MAX;
      for(int i=0; i<n-afterTurns; i++) {
         table[i] = std::max(x[i], x[i+afterTurns]);
	 turnMin = std::min(turnMin, table[i]);
      }

      // for each turn:
      for(int turn=1; turn<numTurns; turn++) {
	 std::cout << turn << " " << turnMin << std::endl;
         turnMin = INT_MAX;
	 for(int i=0; i<n-m+1; i++) {
	    // calculate min of middle section:
	    int midMin = INT_MAX;
	    for(int j=1; j<m-1; j++) {
	       midMin = std::min(midMin, table[i+j]);
	    }
	    table[i] = std::max( x[i] + std::min(midMin, table[i+m-1]) ,
			         std::min( table[i] , midMin) + x[i+m-1]);
	    turnMin = std::min(turnMin, table[i]);
	 }
      }

      std::cout << turnMin << std::endl;
      std::cout << beforeTurns << " " << numTurns << " " << afterTurns << std::endl;
   }
}
