#include <iostream>
#include <map>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
   boost::property<boost::edge_capacity_t, long,
   boost::property<boost::edge_residual_capacity_t, long,
   boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;
typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

typedef boost::graph_traits<graph>::out_edge_iterator out_edge_it;

class edge_adder {
   graph &G;
public:
   explicit edge_adder(graph &G) : G(G) {}
   edge_desc add_edge(int from, int to, long capacity) {
      auto c_map = boost::get(boost::edge_capacity, G);
      auto r_map = boost::get(boost::edge_reverse, G);
      const auto e = boost::add_edge(from, to, G).first;
      const auto rev_e = boost::add_edge(to, from, G).first;
      c_map[e] = capacity;
      c_map[rev_e] = 0; // reverse edge has no capacity!
      r_map[e] = rev_e;
      r_map[rev_e] = e;
      return e;
   }
};


struct Edge {
   int u,v;
   Edge(int u, int v) : u(u),v(v) {};
};
bool operator < (const Edge& e1, const Edge& e2) {
return (e1.u < e2.u or (e1.u==e2.u and e1.v < e2.v));
}

int main() {
   std::ios_base::sync_with_stdio(false);

   int t;
   std::cin >> t;

   for(int tt=0; tt<t; tt++) {
      int n,m;
      std::cin >> n >> m;
      
      std::map<Edge,long> limbs;
      std::map<Edge,edge_desc> edges;
      for(int i=0; i<m; i++) {
         int a,b,c;
	 std::cin >> a >> b >> c;

	 if(limbs.find(Edge(a,b))==limbs.end()) {
	    limbs[Edge(a,b)] = 0;
	 }
	 limbs[Edge(a,b)] += c;
      }

      graph G(n);
      edge_adder adder(G);

      for(auto &it : limbs) {
         const Edge &e = it.first;
	 const long c = it.second;

         const edge_desc &ee = adder.add_edge(e.u, e.v, c);
         //std::cout << ee << " " << e.u << " " << e.v <<std::endl;
	 edges[e] = ee;
      }
      
      long minFlow = 1l << 62;
      int src = -1;
      int dst = -1;
      
      for(int j=1; j<n; j++) {
         int i = 0;
         long flow = boost::push_relabel_max_flow(G, i,j);
	 if(flow < minFlow) {
	    minFlow = flow;
            src = i; dst = j;
	 }	
      }
      for(int i=1; i<n; i++) {
         int j = 0;
         long flow = boost::push_relabel_max_flow(G, i,j);
	 if(flow < minFlow) {
	    minFlow = flow;
            src = i; dst = j;
	 }	
      }
      
      std::cout << minFlow << std::endl;
      
      long flow = boost::push_relabel_max_flow(G, src,dst);

      const auto rc_map = boost::get(boost::edge_residual_capacity, G);

      std::vector<int> queue;
      queue.reserve(n);
      int qIndex = 0;
      std::vector<bool> visited(n,false);
      visited[src] = true;
      int cnt=1;
      queue.push_back(src);

      while(qIndex < queue.size()) {
         int u = queue[qIndex++];

         out_edge_it oe_beg, oe_end;
         for (boost::tie(oe_beg, oe_end) = boost::out_edges(u, G); oe_beg != oe_end; ++oe_beg) {
            assert(boost::source(*oe_beg, G) == u);
	    int v = boost::target(*oe_beg, G);
            
	    if(edges.find(Edge(u,v))==edges.end())  {continue;}
	    edge_desc &e = edges[Edge(u,v)];
            //std::cout << e << " " << u << " " << v << std::endl;

	    if(u!=v and !visited[v] and rc_map[e]>0) {
	       visited[v] = true;
	       cnt++;
	       queue.push_back(v);
	    }
	 }
      }
      
      std::cout << cnt << " "; 
      for(int i=0; i<n; i++) {
         if(visited[i]) {
	    std::cout << i << " ";
	 }
      }
      std::cout << std::endl;
   
   }
}



