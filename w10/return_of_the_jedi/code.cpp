#include <iostream>
#include <vector>
#include <queue>

struct Edge {
   int d, u, v;
   Edge(int d, int u, int v) : d(d), u(u), v(v) {};
};
bool operator < (const Edge& e1, const Edge& e2) {
   return (e1.d < e2.d or (e1.d == e2.d and e1.v < e2.v));
}

struct Test {
   int n,tat;
   std::vector<int> d;

   Test() {
      std::cin >> n >> tat;
      d.resize(n*n,0);

      for(int i=0; i<n; i++) {
         for(int j=i+1; j<n; j++) {
            int dd;
	    std::cin >> dd;
	    d[i*n + j] = dd;
	    d[j*n + i] = dd;
	 }
      }

      //for(int i=0; i<n; i++) {
      //   for(int j=0; j<n; j++) {
      //      std::cout << " " << d[i*n+j];
      //   }
      //   std::cout << std::endl;
      //}
      
      buildNetwork();

      std::cout << 0 << std::endl;
   };

   void buildNetwork() {
      std::vector<bool> visited(n,false);
      std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>> queue;

      queue.push(Edge(0,tat));

      while(queue.size()>0) {
         Edge e = queue.top(); queue.pop();
	 if(!visited[e.v]) {
	    //continue here
	 }
      }
      


   }
};

int main() {
   std::ios_base::sync_with_stdio(false);

   int t;
   std::cin >> t;
   for(int tt=0;tt<t;tt++) {
      Test ttt;
   }
}


