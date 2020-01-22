// flow algorithm:
// source flows to zone with capacity ci -> can enough flow?
// from job to dst there is pj flow, more cannot be won.
// from zone to job, there is an inf edge if job requires zone.
//
// question: is src-zone saturated? = can zone be payed for?

#include <iostream>
#include <vector>
#include <cassert>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
   boost::property<boost::edge_capacity_t, long,
   boost::property<boost::edge_residual_capacity_t, long,
   boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;
typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

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


struct Canton {
   std::vector<int> c;
   std::vector<int> p;
   int z,j;
   std::vector<bool> M;

   std::vector<edge_desc> zoneEdge;
   
   int ret;

   Canton() {
      std::cin >> z >> j;

      graph G(z+j+2);
      edge_adder adder(G);
      int src = z+j;
      int dst = z+j+1;
      
      c.resize(z);
      zoneEdge.reserve(z);
      edge_desc e;
      for(int i=0; i<z; i++) {
	 std::cin >> c[i];
	 e = adder.add_edge(src,i, c[i]);
	 zoneEdge.push_back(e);
      }

      p.resize(j);
      for(int i=0; i<j; i++) {
	 std::cin >> p[i];
	 adder.add_edge(i+z,dst, p[i]);
      }
      
      M.resize(z*j,false);
      for(int i=0; i<j; i++) {
         int ni;
	 std::cin >> ni;
	 for(int n=0; n<ni; n++) {
	    int zz;
	    std::cin >> zz;
	    adder.add_edge(zz, i+z, (1<<30));
	    M[i*j + zz] = true;
	 }
      }

      long flow = boost::push_relabel_max_flow(G, src, dst);
      const auto c_map = boost::get(boost::edge_capacity, G);
      const auto rc_map = boost::get(boost::edge_residual_capacity, G);

      std::vector<bool> takeZone(z,false);
      for(int i=0; i<z; i++) {
         e = zoneEdge[i];
	 long ff = c_map[e] - rc_map[e];
	 if(ff == c[i]) {takeZone[i] = true;}
      }

      long profit = 0;
      long cost = 0;
      std::vector<bool> reallyTakeZone(z,false);
      for(int i=0; i<j; i++) {
         bool take = true;
	 for(int zz=0; zz<z; zz++) {
	    if(M[i*j + zz] and not takeZone[zz]) {take = false;}
	 }

	 if(take) {
            profit+=p[i];
	    std::cout << "take j " << i << std::endl;
	    for(int zz=0; zz<z; zz++) {
	       if(M[i*j + zz]) {
	          if(not reallyTakeZone[zz]) {
		     reallyTakeZone[zz] = true;
		     cost += c[zz];
	             std::cout << "take z " << zz << std::endl;
		  }
	       }
	    }
	 }
      }
     
      std::cout << "f: " << flow << std::endl;
      std::cout << "p: " << profit << " c: " << cost << std::endl; 

      ret = profit-cost;
      assert(ret>=0);
   }

   int run() {
      return ret;
   }
};


int main() {
   std::ios_base::sync_with_stdio(false);

   int t;
   std::cin >> t;
   for(int tt=0; tt<t; tt++) {
      Canton c;
      std::cout << c.run() << std::endl;
   }
}
