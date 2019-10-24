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

void make_it_flow() {
  int n; int m;
  std::cin >> n >> m;
  
  graph G(n); // n nodes for players
  edge_adder adder(G);

  // Add special vertices source and sink
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_target = boost::add_vertex(G);
  
  std::vector<int> points(n); // points already gained
  
  int missingGames(0);
  for(int i=0;i<m;i++) {
    int a;int b;int c;
    std::cin >> a >> b >> c;
    if(c==0) {
      const vertex_desc game = boost::add_vertex(G);
      adder.add_edge(v_source,game,1);
      adder.add_edge(game,a,1);
      adder.add_edge(game,b,1);
      missingGames++;
    } else if (c==1) {
      points[a]++;
    } else {
      points[b]++;
    }
  }
  
  int missingPoints(0);
  for(int i=0;i<n;i++) {
    int p;
    std::cin >> p;
    if(p < points[i]) {
      std::cout << "no" << std::endl;
      return;
    }
    missingPoints+=p-points[i];
    adder.add_edge(i,v_target,p-points[i]);
  }
  
  // Calculate flow from source to sink
  // The flow algorithm uses the interior properties (managed in the edge adder)
  // - edge_capacity, edge_reverse (read access),
  // - edge_residual_capacity (read and write access).
  long flow = boost::push_relabel_max_flow(G, v_source, v_target);
  std::cout << "The total flow is " << flow << "\n";
  std::cout << "mG " << missingGames << " missingPoints " << missingPoints << std::endl;

  // Retrieve the capacity map and reverse capacity map
  //const auto c_map = boost::get(boost::edge_capacity, G);
  //const auto rc_map = boost::get(boost::edge_residual_capacity, G);

  // Iterate over all the edges to print the flow along them
  //auto edge_iters = boost::edges(G);
  //for (auto edge_it = edge_iters.first; edge_it != edge_iters.second; ++edge_it) {
  //  const edge_desc edge = *edge_it;
  //  const long flow_through_edge = rc_map[edge] - c_map[edge];
  //  std::cout << "edge from " << boost::source(edge, G) << " to " << boost::target(edge, G)
  //            << " runs " << flow_through_edge
  //            << " units of flow (negative for reverse direction). \n";
  //}
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
