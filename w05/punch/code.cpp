#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <climits>
#include <queue>

// greedy c/v does not work: 4 people (3/3, 5/4) -> better one expensive than 2 cheaper

typedef std::priority_queue<int,std::vector<int>,std::greater<int>> PQ;

void update(long &minCost,long &maxBev, long newCost, long newBev) {
   if(newBev==0) {return;}
   if(newCost < minCost) {
      minCost = newCost;
      maxBev = newBev;
   }
   if(newCost == minCost and maxBev < newBev) {
      maxBev = newBev;
   }
}

int main () {
   std::ios_base::sync_with_stdio(false);
   
   int t(0);
   std::cin >> t;
   
   for(int tt=0; tt<t; tt++) {
      int n(0); int k(0);
      std::cin >> n >> k;
      
      std::vector<int> cost(n);
      std::vector<int> volume(n);
      for(int i=0;i<n;i++) {
         std::cin >> cost[i] >> volume[i];
      }

      std::vector<int> numBev(2*k,0); // max num Bev for price (see below)
      std::vector<int> numBevOld(2*k,0); // max num Bev for price (see below)
      std::vector<long> mixCost(2*k,INT_MAX); // min price to reach this volume
      std::vector<long> mixCostOld(2*k,INT_MAX); // min price to reach this volume

      //idea: for volume v: look at v-volume[i] with and without item i
      
      int vv=1;
      for(int v=volume[0];v<2*k;v+=volume[0]){
         numBev[v] = 1;
	 mixCost[v] = vv*cost[0];
	 vv++;
      }
      for(int i=1;i<n;i++) {
	 std::swap(mixCost,mixCostOld);
	 std::swap(numBev,numBevOld);
         for(int v=0;v<2*k;v++) {
	    long minCost = mixCostOld[v];
	    long maxBev = numBevOld[v];
	    if(v==volume[i]) {
	       update(minCost,maxBev, cost[i],1L);
	    }
	    if(v>=volume[i]) {
	       int vv = v-volume[i];
	       // look at smaller volume, extend with more of i
	       update(minCost,maxBev, mixCost[vv]+cost[i],numBev[vv]);
	       
	       // look at smaller, and without i
	       if(numBevOld[vv]>0) {
	          update(minCost,maxBev, mixCostOld[vv]+cost[i],numBevOld[vv]+1);
	       }
	    }
	    mixCost[v] = minCost;
	    numBev[v] = maxBev;
	 }
      }
      
      long minCost = INT_MAX;
      long maxBev = 0;

      for(int v=k;v<2*k;v++) {
	 if(numBev[v]>0) {
            update(minCost,maxBev,mixCost[v],numBev[v]);
	 }
      }
      
      std::cout << minCost << " " << maxBev << std::endl;
   }
}
