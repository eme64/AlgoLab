#include <iostream>
#include <vector>
#include <map>

struct Edge {
   int u,v;
   Edge(int u,int v) : u(u),v(v) {};
};

bool operator < (const Edge& e1, const Edge& e2) {
   return (e1.u < e2.u or (e1.u == e2.u and e1.v < e2.v));
}

struct Test{
   int n,m,k;
   long x;
   std::map<Edge,long> edges;
   Test() {
      std::cin >> n >> m >> x >> k;

      std::vector<Edge> ein;
      ein.reserve(m);
      std::vector<long> points;
      points.reserve(m);

      std::vector<int> indeg(n,0);
      for(int i=0; i<m; i++) {
         int u,v;
	 long p;
	 std::cin >> u >> v >> p;
	 Edge e(u,v);
	 ein.push_back(e);
	 points.push_back(p);
	 indeg[u]++;
      }
      std::vector<int> id(n,0);
      for(int i=0; i<n; i++) {
         if(indeg[i]>0) {id[i]=i;}
      }

      for(int i=0; i<m; i++){
	 Edge &eold = ein[i];
	 long p = points[i];
         int u = id[eold.u];
         int v = id[eold.v];

	 Edge e(u,v);
	 const auto &it = edges.find(e);
	 if(it == edges.end()) {
	    edges[e] = p;
	 } else {
	    edges[e] = std::max(edges[e],p);
	 }
      }
      //for(auto &it : edges) {
      //   const Edge &e = it.first;
      //   const long p = it.second;
      //   std::cout << "e " << e.u << " " << e.v << " " << p << std::endl;
      //}


      int kk=0;
      long kkpts = 0;
      long small = -(1l << 62);
      std::vector<long> table(n,small);
      table[0] = 0;
      std::vector<long> newtable(n,0);
      while(kk<k and kkpts<x) {
	 std::fill(newtable.begin(),newtable.end(),small);
         
	 for(auto &it : edges) {
	    const Edge &e = it.first;
	    const long p = it.second;

	    const long newt = std::max(p + table[e.u], newtable[e.v]);
	    newtable[e.v] = newt;
            kkpts = std::max(kkpts,newt);
	 }
	 //std::cout << kk << " " << kkpts << std::endl;
	 
	 std::swap(newtable, table);
	 kk++;
      }
      
      if(kkpts < x) {
         std::cout << "Impossible" << std::endl;
      } else {
         std::cout << kk << std::endl;
      }
   }
};

int main() {
   std::ios_base::sync_with_stdio(false);
   
   int t;
   std::cin >> t;
   for(int tt=0; tt<t; tt++) {Test ttt;}
}
