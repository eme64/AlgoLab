#include <iostream>
#include <vector>


struct Test {
   int n;
   std::vector<long> a;
   std::vector<long> b;
   std::vector<long> pa;
   std::vector<long> pb;
   Test() {
      std::cin >> n;
      a.resize(n,0);
      b.resize(n,0);

      for(int i=0; i<n; i++) {std::cin >> a[i];}
      for(int i=0; i<n; i++) {std::cin >> b[i];}
      
      // prefix sum: easily get cost for one side.
      pa.resize(n+1,0);
      pb.resize(n+1,0);
      long suma = 0;
      long sumb = 0;
      for(int i=0; i<n; i++){
	 suma+=a[i]-1; pa[i+1] = suma;
	 sumb+=b[i]-1; pb[i+1] = sumb;
      }

      // n4: table, for x,y look for cheapest extension option O(n2)
      // basically enumerates all possible loads
      std::vector<int> T((n+1)*(n+1), -1);
      T[0] = 0;
      for(int x=1; x<=n; x++) { // stack pos a
         for(int y=1; y<=n; y++) { // stack pos b
            long val = (pa[x]-pa[0]) * (pb[y]-pb[0]);// if take all now
	    for(int xx=1; xx<x; xx++) {
	       for(int yy=1; yy<y; yy++) {
	          long last = T[xx*(n+1) + yy];// rest of turns
		  long now = (pa[x]-pa[xx]) * (pb[y]-pb[yy]);
		  long total = last + now;
		  val = std::min(val, total);
	       }
	    }
	    T[x*(n+1) + y] = val;
	    //std::cout << " " << val;
         }
	 //std::cout << std::endl;
      }

      std::cout << T[n*(n+1) + n] << std::endl;
   }
};


int main() {
   std::ios_base::sync_with_stdio(false);
   
   int t;
   std::cin >> t;
   for(int tt=0; tt<t; tt++) {
      Test ttt;
   }
}



