#include <iostream>
#include <vector>

struct Case {
   int n,m;
   long k;
   std::vector<int> value;
   std::vector<int> maxVal;
   Case() {
      std::cin >> n >> m >> k;
      value.resize(n);
      //segment.resize(n,-1);
      maxVal.resize((m+1)*(n+1),-1);
      // [y*n + x] -> max value possible up to this with y attackers and line up to x
      for(int i=0; i<n+1; i++) {
         maxVal[0*(n+1) + i] = 0; // always possible with 0 attackers to get 0 value
      }
      int j = 0; // window from, inclusive
      long sum = 0; // sum of values in window
      for(int i=0; i<n; i++) {
         std::cin >> value[i];
         sum += value[i];
	 while(sum > k) {sum-=value[j++];}

	 if(sum == k) {
            //segment[i] = j;
	    int locGain = i-j+1;
	    for(int a=1; a<=m; a++) {
	       int prevGain = maxVal[a*(n+1) + (i)];
	       int otherGain = maxVal[(a-1)*(n+1) + (j)];
	       if(otherGain >= 0) {
	          maxVal[a*(n+1) + (i+1)] = std::max(prevGain, otherGain+locGain);
	       } else {
	          maxVal[a*(n+1) + (i+1)] = prevGain;
	       }
	    }

	 } else {
	    for(int a=1; a<=m; a++) {
	       int prevGain = maxVal[a*(n+1) + (i)];
	       maxVal[a*(n+1) + (i+1)] = prevGain;
	    }
	 }
      }
      //for(int i=0; i<n; i++) {
      //   std::cout << segment[i] << " ";
      //}
      //std::cout << std::endl;
      
      // data-structure:
      // per number of segments
      // sort by end position
      // indicate max value acheivable up to it
      
      //for(int y=0; y<=m; y++) {
      //   for(int x=0; x<=n; x++) {
      //      std::cout << maxVal[y*(n+1) + x] << " ";
      //   }
      //   std::cout << std::endl;
      //}
      int res = maxVal[m*(n+1) + n];
      if(res >= 0) {
         std::cout << maxVal[m*(n+1) + n] << std::endl;
      } else {
         std::cout << "fail" << std::endl;
      }
   }
};

int main() {
   std::ios_base::sync_with_stdio(false);

   int t;
   std::cin >> t;
   for(int tt=0; tt<t; tt++) {
      Case c;
   }
}



