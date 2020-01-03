#include <iostream>
#include <vector>

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

struct Link {
   int gg,ss;
   Link(int gg, int ss) : gg(gg), ss(ss) {};
};
bool operator < (const Link& e1, const Link& e2) {
   return (e1.gg < e2.gg or (e1.gg==e2.gg and e1.ss < e2.ss));
}

int main() {
   std::ios_base::sync_with_stdio(false);

   int t;
   std::cin >> t;

   for(int tt=0; tt<t; tt++) {
      int g,s,l;
      std::cin >> g >> s >> l;
      
      graph G(g+s);
      edge_adder adder(G);
      const vertex_desc v_source = boost::add_vertex(G);
      const vertex_desc v_target = boost::add_vertex(G);
      
      std::map<Link, bool> isMatch;
      std::map<Link, edge_desc> edges;
      std::vector<int> match(g+s,-1);
      std::vector<bool> vIsVisited(g+s,false);

      for(int i=0; i<g; i++) {
         adder.add_edge(v_source,i,1);
      }
      for(int i=0; i<s; i++) {
         adder.add_edge(g+i,v_target,1);
      }
      for(int i=0; i<l; i++) {
         int gg,ss;
	 std::cin >> gg >> ss;
         const edge_desc& e = adder.add_edge(gg,g+ss,1);
	 isMatch[Link(gg,ss)] = false;
	 edges[Link(gg,ss)] = e;
      }

      long flow = boost::push_relabel_max_flow(G, v_source, v_target);
      
      const auto c_map = boost::get(boost::edge_capacity, G);
      const auto rc_map = boost::get(boost::edge_residual_capacity, G);
      
      for(auto &it : edges) {
         const Link& ll= it.first;
         const edge_desc& e = it.second;
	 long f = c_map[e] - rc_map[e];
	 if(f>0) {
	    isMatch[ll] = true;
	    match[ll.gg] = ll.ss;
	    match[ll.ss+g] = ll.gg;
	    //std::cout << ll.gg << " " << ll.ss << "  " << f << std::endl;
	 }
      }
      
      std::vector<int> queue;
      queue.reserve(g+s);
      int qIndex=0;

      // mark left unmatched:
      for(int i=0; i<g; i++) {
         if(match[i]==-1) {queue.push_back(i); vIsVisited[i]=true;}
      }

      while(queue.size() > qIndex) {
         int v = queue[qIndex++];
	 //std::cout << "visit: " << v << std::endl;
         
	 if(v<g) {
	    int gg = v;
            out_edge_it oe_beg, oe_end;
            for (boost::tie(oe_beg, oe_end) = boost::out_edges(gg, G); oe_beg != oe_end; ++oe_beg) {
               assert(boost::source(*oe_beg, G) == gg);
	       int ss = boost::target(*oe_beg, G);
	       if(ss-g < s and !isMatch[Link(gg,ss-g)] and !vIsVisited[ss]) {
	          vIsVisited[ss] = true;
		  queue.push_back(ss);
		  //std::cout << "lr: " << gg << " " << (ss-g) << std::endl;
	       }
            }
	 } else {
	    int ss = v-g;
	    int m = match[v];
	    if(m!=-1 and !vIsVisited[m]) {
	       vIsVisited[m] = true;
	       queue.push_back(m);
	       //std::cout << "rl: " << ss << " " << m << std::endl;
	    }
	 }
      }

      int cnt=0;
      std::vector<int> res;
      res.reserve(g+s);
      for(int i=0; i<g; i++) {
         if(!vIsVisited[i]) {cnt++;res.push_back(i);}
      }
      int cnt2 = 0;
      for(int i=0; i<s; i++) {
         if(vIsVisited[i+g]) {cnt2++;res.push_back(i);}
      }
      std::cout << cnt << " " << cnt2 << std::endl;
      for(int i=0; i<res.size(); i++) {
	 std::cout << res[i] << " ";
      }
      if(res.size()>0) {std::cout << std::endl;}
   }
}
