#include <iostream>
#include <iomanip>
#include <vector>


int main() {
   std::ios_base::sync_with_stdio(false);
   
   std::cout << std::fixed << std::setprecision(5);

   int t;
   std::cin >> t;
   for(int tt=0; tt<t; tt++) {
      int n,k,m;
      std::cin >> n >> k >> m;

      std::vector<double> prob(n,0);

      for(int i=0; i<n; i++) {
         std::cin >> prob[n-i-1];
      }

      std::vector<double> T(m+1,0);
      // probability to win with i in this round.
      // all zero, since after last day must have m
      // implicitly, m and over are 1.
      
      std::vector<double> Tnew(m+1,0);
      T[m] = 1;
      Tnew[m] = 1;

      for(int r=0; r<n; r++) {
         double p = prob[r];
	 double pinv = 1.0-p;

	 for(int i=0; i<m; i++) {
	    double newp = 0;
	    int dd = std::min(i,m-i);
	    for(int d = 0; d<=dd; d++) {
	       double locp = p*T[i+d] + pinv*T[i-d];
	       newp = std::max(newp, locp);
	    }
	    Tnew[i] = newp;
	    //std::cout << newp << " ";
	 }
	 //std::cout << std::endl;
	 std::swap(T,Tnew);
      }
      
      std::cout << T[k] << std::endl;
   }
}




