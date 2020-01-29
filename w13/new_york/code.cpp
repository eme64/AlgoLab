#include <iostream>
#include <vector>
#include <set>
#include <cassert>

struct E {
   int a,b;
   E(int a, int b) : a(a),b(b) {}
};

struct Item {
   int h,i;
   Item(int h, int i) : h(h), i(i) {}
};

bool operator < (const Item& e1, const Item& e2) {
   return (e1.h < e2.h or (e1.h == e2.h and e1.i < e2.i));
}

struct Test {
   int n,m,k;
   std::vector<int> h;
   Test() {
      std::cin >> n >> m >> k;
     
      int maxH = 0;
      h.resize(n);
      for(int i=0; i<n; i++) {
         std::cin >> h[i];
	 maxH = std::max(maxH, h[i]);
      }

      std::vector<int> nChildren(n,0);
      std::vector<E> edge;
      edge.reserve(n);
      for(int i=0; i<n-1; i++) {
         int a,b;
	 std::cin >> a >> b;
	 nChildren[a]++;
	 edge.push_back(E(a,b));
      }
      int sum = 0;
      std::vector<int> offset(n,0);
      std::vector<int> child(n);
      for(int i=0; i<n; i++) {
         offset[i] = sum;
	 sum+=nChildren[i];
      }
      std::vector<int> index(n,0);
      for(const E& e : edge) {
         int i = index[e.a]++;
	 child[offset[e.a] + i] = e.b;

	 //std::cout << "edge: " << e.a << " " << e.b << std::endl;
      }

      std::vector<int> stack(n,0);
      std::vector<int> stackC(n,0); // how many children were handled yet?
      int stackI = 0;
      stack[0] = 0;
      stackC[0] = 0;
      
      std::vector<bool> feasible(n,false);
      std::set<Item> active;

      while(stackI>=0) {
	 assert(stackI <= n);
	 const int i = stack[stackI];
         if(stackC[stackI] == 0){
            //std::cout << stackI << " pre: " << i << " " << nChildren[i]<< std::endl;
            
	    // potentially cache an anchestor
	    if(stackI >= m) {
	       int ic = stack[stackI-m];
	       //std::cout << "-------cache " << ic << std::endl;
	       active.erase(Item(h[ic], ic));
	    }

	    //add me:
	    active.insert(Item(h[i], i));

            // validate min/max:
	    const auto &imin = active.begin();
	    const auto &imax = active.rbegin();
	    //std::cout << " -- " << imin->h << " " << imin->i << " - " << imax->h << " " << imax->i << std::endl;
	    assert(imin!=active.end());
	    assert(imax!=active.rend());
	    const int diff = imax->h - imin->h;
	    //std::cout << " ## " << diff << std::endl;

	    if(diff <= k and stackI>=m-1) {
	       int fi = stack[stackI-m+1];
	       feasible[fi] = true;
	    }
	 }
	 if(stackC[stackI] < nChildren[i]) {
            int c = child[offset[i] + stackC[stackI]];
            //std::cout << stackI << " child: " << c << std::endl;
            stack[stackI+1] = c;
	    stackC[stackI+1] = 0;
            stackC[stackI]++;
	    stackI++;
	 } else {
            //std::cout << stackI << " post: " << i << std::endl;
	    
	    // potentially uncache an anchestor
	    if(stackI >= m) {
	       int ic = stack[stackI-m];
	       //std::cout << "++++++++uncache " << ic << std::endl;
	       active.insert(Item(h[ic], ic));
	    }

	    // rm me:
	    active.erase(Item(h[i], i));
	    
	    stackI--;
	 }
      }

      int fCnt = 0;
      for(int i=0; i<n; i++) {
         if(feasible[i]) {std::cout << i << " "; fCnt++;}
      }
      if(fCnt == 0) {std::cout << "Abort mission" << std::endl;} else {std::cout << std::endl;}
   }
};

int main() {
   std::ios_base::sync_with_stdio(false);

   int t;
   std::cin >> t;
   for(int i=0; i<t; i++) {Test tt;}
}

