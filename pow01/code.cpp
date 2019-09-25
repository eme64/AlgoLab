// Idea:

#include <iostream>
#include <vector>
#include <algorithm>
int main() {
   std::ios_base::sync_with_stdio(false);
   
   int t(0);
   std::cin >> t;

   for(int tt = 0; tt<t; tt++) {
      int n(0);
      int k(0);
      std::cin >> n >> k;
      
      std::vector<int> p;
      p.resize(n+1);
      p[0]=0;
      int sum(0);

      for(int i = 0; i<n; i++) {
         int v(0);
	 std::cin >> v;
	 sum+=v;
	 p[i+1] = sum;
      }
      
      int minvalabs = p[1];
      int minval = std::abs(minvalabs-k);
      int ii(1);
      int jj(1);
      
      int i(1);
      int j(1);
      while(i<=n) {
	 int valabs = p[j]-p[i-1];
         int val = std::abs(valabs-k);
	 if(minval > val) {
	    ii = i; jj = j;
	    minval = val;
	    minvalabs = valabs;
	 }
	 if(valabs < k) {
	    j++;
	    if(j>n) {
	       i = n+1;
	       break;
	    }
	 } else if(valabs > k) {
	    i++;
	 } else {
            i = n+1;
	    break;
	 }
      }
      std::cout << ii-1 << " " << jj-1 << std::endl;

      
      /*
      for(int i = 0; i<n; i++) {
         for(int j = i; j<n; j++) {
	    int val = std::abs(p[j+1]-p[i]-k);
	    if(val < minval) {
	    	ii = i;
		jj = j;
		minval = val;
		//std::cout << ii << " " << jj << " " << minval << std::endl;
	    }
	 }
      }

      std::cout << ii << " " << jj << std::endl;
      */
      
      
      /*
      for(int i=0; i<n; i++) {
         int toFind = p[i]+k;
         int l(i+1);
	 int r(n+1);
	 while(l<r) {
	    int m = (l+r)/2;
	    if(p[m]<toFind) {
	       l = m+1;
	    } else if(p[m]>toFind) {
	       r = m;
	    } else {
	       r = m;
	       l = m;
	    }
	 }
	 
	 int anchor = l;
	 int res = std::abs(toFind-p[anchor]);
	 while(true) {
	    int newRes = std::abs(toFind-p[anchor-1]);
	    if(newRes<=res) {
	       res = newRes;
	       anchor--;
	    } else {
	       break;
	    }
	 }
	 int top = l;
	 int res2 = std::abs(toFind-p[anchor]);
	 while(true) {
	    int newRes = std::abs(toFind-p[top+1]);
	    if(newRes<res2) {
	       res2 = newRes;
	       top++;
	    } else {
	       break;
	    }
	 }
	 //std::cout << l << " " << anchor << " " << top << std::endl;
	 //std::cout << res << res2 << std::endl;
	 //
	 if(minval > res) {
	    minval = res;
	    ii = i;
	    jj = anchor;
	 }
      }
      std::cout << ii << " " << jj-1 << std::endl;
      */
   }
}
