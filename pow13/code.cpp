#include <iostream>
#include <vector>
#include <map>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Finite_faces_iterator Face_iterator;
typedef K::Point_2 P;
typedef Triangulation::Vertex_handle V;

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS> graph;
typedef boost::graph_traits<graph>::edge_iterator edge_it;



struct Test {
   int n, r; K::FT r2;
   std::vector<P> p;
   std::map<P,int> p2i;
   Test () {
      std::cin >> n >> r;
      r2 = K::FT(r)*K::FT(r);
      p.reserve(n);
      //std::cout << "R: " << r << std::endl;
      for(int i=0; i<n; i++) {
         int x,y;
	 std::cin >> x >> y;
	 P pp(x,y);
	 p.push_back(pp);
	 p2i[pp] = i;
	 //std::cout << "pt: " << i << " " << x << " " << y << std::endl;
      }
      int big = 1 << 30;
      {  P pp(-big,-big); p.push_back(pp); p2i[pp] = -1;  }
      {  P pp( big, big); p.push_back(pp); p2i[pp] = -1;  }
      {  P pp(-big, big); p.push_back(pp); p2i[pp] = -1;  }
      {  P pp( big,-big); p.push_back(pp); p2i[pp] = -1;  }

      int lowk = 0;
      int highk = std::min(n/2+1,n);

      while(lowk+1<highk) {
         int m=(lowk+highk)/2;
	 if(isFeasible(m)) {
	    lowk = m;
	 } else {
	    highk = m;
	 }
      }
      
      //assert(isFeasible(lowk));
      //assert(!isFeasible(lowk+1));

      std::cout << lowk << std::endl;
   }

   bool isFeasible(int k) {
      //std::cout << "try " << k << std::endl;
      if(k==0) {return true;}
      Triangulation t;
      t.insert(p.begin()+k, p.end());
      
      graph G(n);

      for (Face_iterator f = t.finite_faces_begin(); f != t.finite_faces_end(); ++f) {
	 std::vector<int> index;
	 for(int i=0; i<3; i++) {
	    int ind = p2i[f->vertex(i)->point()];
	    if(ind>=0) {index.push_back(ind);}
	 }

	 for(int i=0; i<index.size(); i++) {
	    for(int j=i+1; j<index.size(); j++) {
	       const int ii = index[i];
	       const int ij = index[j];
	       const P &pi(p[ii]);
	       const P &pj(p[ij]);
               if(r2 >= CGAL::squared_distance(pi, pj)){
	          //std::cout << ii << " " << ij << std::endl;
		  boost::add_edge(ii,ij, G);
		  boost::add_edge(ij,ii, G);
	       }
	    }
	 }
      }

      std::vector<int> scc_map(n);
      int nscc = boost::strong_components(G,
		      boost::make_iterator_property_map(scc_map.begin(), boost::get(boost::vertex_index, G)));
      
      std::vector<int> cnt(nscc, 0);
      int cntMax = 0;
      for(int i=k; i<n; i++) {
	 int comp = scc_map[i];
	 //std::cout << "comp: " << i << " " << comp << std::endl;
         int tmp = cnt[comp]+1;
	 cntMax = std::max(tmp, cntMax);
	 cnt[comp] = tmp;
      }
      //std::cout << "max: " << cntMax << " " << k << std::endl;

      return (cntMax >= k);
   }
};


int main() {
   std::ios_base::sync_with_stdio(false);

   int t;
   std::cin >> t;
   for(int i=0; i<t; i++) {Test tt;}
}



