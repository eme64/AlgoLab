#include <iostream>
#include <vector>
#include <queue>
#include <cstdint>

struct Edge {
   int d, u, v;
   Edge(int d, int u, int v) : d(d), u(u), v(v) {};
};
bool operator > (const Edge& e1, const Edge& e2) {
   return (e1.d > e2.d or (e1.d == e2.d and e1.v > e2.v));
}

struct Test {
   int n,tat;
   std::vector<int> d;
   std::vector<Edge> mst;
   long mstCost;
   
   std::vector<int> deg;
   std::vector<int> offset;
   std::vector<int> graph;
   std::vector<bool> graphM;
   
   std::vector<int> maxLink;

   Test() {
      std::cin >> n >> tat;
      tat--;
      d.resize(n*n,0);

      for(int i=0; i<n; i++) {
         for(int j=i+1; j<n; j++) {
            int dd;
	    std::cin >> dd;
	    d[i*n + j] = dd;
	    d[j*n + i] = dd;
	 }
      }

     
      buildNetwork();
      
      long minVal = (1l << 60);
      for(int i=0; i<n; i++) {
         for(int j=0; j<n; j++) {
            //std::cout << " " << d[i*n+j];
            if(i!=j and !graphM[i*n+j]) {
	       int locD = d[i*n + j];
	       int maxD = maxLink[i*n + j];
	       long total = mstCost - maxD + locD;
	       minVal = std::min(minVal, total);
	    }
	 }
         //std::cout << std::endl;
      }
 

      std::cout << minVal << std::endl;
   };

   void buildNetwork() {
      mst.reserve(n-1);
      std::vector<bool> visited(n,false);
      std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>> queue;

      queue.push(Edge(0,tat,tat));

      while(queue.size()>0) {
         Edge e = queue.top(); queue.pop();
	 if(!visited[e.v]) {
	    visited[e.v] = true;
	    if(e.u!=e.v) {mst.push_back(e);}
	    for(int i=0; i<n; i++) {
	       if(i!=e.v and !visited[i]) {
	          queue.push(Edge(d[i*n + e.v],e.v,i));
	       }
	    }
	 }
      }
      
      deg.resize(n,0);
      offset.resize(n,0);
      for(int i=0; i<mst.size(); i++) {
         //std::cout << "mst: " << mst[i].u << " " << mst[i].v << " " << mst[i].d << std::endl;
         deg[mst[i].u]++;
         deg[mst[i].v]++;
      }
      int sum=0;
      for(int i=0; i<n; i++) {
         offset[i] = sum;
	 sum+=deg[i];
      }
      std::vector<int> ind(n,0);
      graph.resize(sum,0);
      graphM.resize(n*n,false);
      mstCost = 0;
      for(int i=0; i<mst.size(); i++) {
         int u = mst[i].u;
         int v = mst[i].v;
	 graph[offset[u] + ind[u]++] = v;
	 graph[offset[v] + ind[v]++] = u;
	 graphM[u*n + v] = true;
	 graphM[v*n + u] = true;
	 mstCost += d[u*n + v];
      }
      
      maxLink.resize(n*n,0);
      std::vector<int> q;
      q.reserve(n);
      for(int s=0; s<n; s++) {
         std::fill(visited.begin(),visited.end(),false);
         q.clear();
	 q.push_back(s);
	 visited[s] = true;
	 int qIndex = 0;
	 while(q.size() > qIndex) {
	    int u = q[qIndex++];
	    for(int i = offset[u]; i<offset[u]+deg[u]; i++) {
	       int v = graph[i];
	       if(!visited[v]) {
	          visited[v] = true;
		  q.push_back(v);
		  int locD = d[u*n + v];
		  int maxD = maxLink[s*n + u];
		  maxLink[s*n + v] = std::max(locD, maxD);
	       }
	       //std::cout << u << " " << v << std::endl;
	    }

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


