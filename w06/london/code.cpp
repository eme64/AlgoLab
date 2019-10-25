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

int letterToInt(char letter) {
  return (int)(letter - 'A');
}

int pairToVertex(int a, int b) {
  return a*26+b;
}

void make_it_flow() {
  int h; int w; std::string letter;
  std::cin >> h >> w;
  std::cin >> letter;
  int n = letter.size();

  std::vector<int> nLetters(26,0);
  for(int i=0; i<n; i++) {
    nLetters[letterToInt(letter[i])]++;
  }
  
  // ideas:
  //  - power each piece by 1
  //  - route each piece to a letter
  //  - route each letter to sink with capacity of number of this letter required
  //  - order of letters is irrelevant.
   
  graph G(26+26*26); // nodes for letters
  edge_adder adder(G);

  // Add special vertices source and sink
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_target = boost::add_vertex(G);
  
  for(int i=0;i<26;i++) {
     adder.add_edge(i,v_target,nLetters[i]);
  }
  
  std::vector<std::string> front(h);
  for(int i=0;i<h;i++) {
    std::cin >> front[i];
  }
  
  std::vector<int> pairs(26*26,0);
  
  for(int i=0;i<h;i++) {
    std::string back;
    std::cin >> back;

    for(int j=0;j<w;j++) {
      int lf = letterToInt(front[i][j]);
      int lb = letterToInt(back[w-j-1]);
      int pair = pairToVertex(lf,lb);
      pairs[pair]++;
    }
  }
  for(int i=0;i<26;i++) {
    for(int j=0;j<26;j++) {
      int pair = pairToVertex(i,j);
      adder.add_edge(26+pair,i,n);
      adder.add_edge(26+pair,j,n);
      adder.add_edge(v_source,26+pair,pairs[pair]);
    }
  }



  long flow = boost::push_relabel_max_flow(G, v_source, v_target);
  std::cout << ((flow==n)?"Yes":"No") << std::endl;
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
