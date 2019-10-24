#include <vector>
#include <iostream>
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

// Custom edge adder class, highly recommended
class edge_adder {
  graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

int crackVertexOut(int i) {
  return 2*i;
}
int crackVertexIn(int i) {
  return 2*i+1;
}

void make_it_flow() {
  int w; long n;
  std::cin >> w >> n;
  
  // ideas:
  //  - at most one right and one left of crack (forms a node)
  //  - flow into left brick, flow out of right sides
  // hence:
  //  - two nodes per crack (in/out) -> capacity 1
  //  - brick connects one crack to next
    
  graph G(2*w+4); // n nodes for players
  edge_adder adder(G);

  // Add special vertices source and sink
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_target = boost::add_vertex(G);
  
  adder.add_edge(v_source,crackVertexOut(0),n); // hook up crack 0 and crack w
  adder.add_edge(crackVertexIn(w),v_target,n);

  for(int i=1;i<w;i++) { // set capacity per vertex
    adder.add_edge(crackVertexIn(i),crackVertexOut(i),1);
  }
  
  for(long i=0;i<n;i++) {
    int a;int b;
    std::cin >> a >> b;
    if(a>b) {std::swap(a,b);}
    adder.add_edge(crackVertexOut(a),crackVertexIn(b),1);
  }
  
  long flow = boost::push_relabel_max_flow(G, v_source, v_target);
  std::cout << flow << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for(int tt=0;tt<t;tt++) {
    make_it_flow();
  }

  return 0;
}
