#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Finite_faces_iterator Face_iterator;

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS> graph;
typedef boost::graph_traits<graph>::edge_iterator edge_it;


#include <boost/graph/kruskal_min_spanning_tree.hpp>
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
boost::no_property, boost::property<boost::edge_weight_t, long> > weighted_graph;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;

struct Edge {
   int u,v;
   Edge(int u,int v) : u(u),v(v) {};
};

bool operator < (const Edge& e1, const Edge& e2) {
   return (e1.u < e2.u or (e1.u==e2.u and e1.v<e2.v));
}

struct Mission {
   int i0,i1;
   long dd;
   Mission(int i0, int i1, long dd) : i0(i0),i1(i1),dd(dd) {};
};

void compComponents(int n, long w, std::map<Edge,long> &edges, std::vector<int> &comp) {
   graph G(n);
   for(auto &it : edges) {
      const Edge &e = it.first;
      const long dd = it.second;
      
      if(dd <= w) {
         boost::add_edge(e.u,e.v,G);
         boost::add_edge(e.v,e.u,G);
      }
   }
   int nscc = boost::strong_components(G,
      boost::make_iterator_property_map(comp.begin(), boost::get(boost::vertex_index, G)));
}

bool checkEnergy(int n, long w, std::map<Edge,long> &edges, std::vector<int> &comp, std::vector<Mission> &missions) {
   compComponents(n,w,edges,comp);
   for(Mission &m : missions) {
      if(w < m.dd) {return false;}
      if(comp[m.i0] != comp[m.i1]) {return false;}
   }
   return true;
}

long findMinEnergy(int n, std::map<Edge,long> &edges, std::vector<int> &comp, std::vector<Mission> &missions) {
   
   long maxdd = 0;
   for(Mission &m : missions) {
      maxdd = std::max(maxdd, m.dd);
   }   

   std::vector<long> values;
   values.reserve(edges.size()+1);
   values.push_back(maxdd);
   for(auto &it : edges) {
      values.push_back(it.second);
   }
   std::sort(values.begin(),values.end());
   int i = 0;
   int j = values.size()-1;
   while(values[i]<maxdd and i<j) {i++;};

   while(i<j) {
      long m = (i+j)/2;
      if(checkEnergy(n,values[m],edges,comp,missions)) {
         j = m;
      } else {
         i = m+1;
      }
   }

   return values[i];
}
long findMinEnergyOld(int n, std::map<Edge,long> &edges, std::vector<int> &comp, std::vector<Mission> &missions) {
   
   bool e0 = checkEnergy(n,0,edges,comp,missions);
   if(e0) {return 0l;}
   
   long lower = 0;
   long upper = 1;

   while(!checkEnergy(n,upper,edges,comp,missions)) {
      lower = upper;
      upper *= 100;
   }
   
   while(true) {
      long m = (lower+upper)/2;
      if(m==lower) {break;}
      if(checkEnergy(n,m,edges,comp,missions)) {
         upper = m;
      } else {
         lower = m;
      }
   }

   return upper;
}

int main() {
   std::ios_base::sync_with_stdio(false);

   int t;
   std::cin >> t;

   for(int tt=0; tt<t; tt++) {
      int n,m;
      long p;
      std::cin >> n >> m >> p;
      
      int big = 1 << 27;

      std::vector<K::Point_2> pts;
      pts.reserve(n+4);
      std::map<K::Point_2,int> ptoi;
      std::map<Edge,long> edges;
      std::vector<int> xx;
      std::vector<int> yy;
      xx.reserve(n);
      yy.reserve(n);

      for(int i=0; i<n; i++) {
         int x,y;
	 std::cin >> x >> y;
	 xx[i] = x;
	 yy[i] = y;
	 K::Point_2 p(x,y);
	 pts.push_back(p);
	 ptoi[p] = i;
      }
      {
	 int x = big;
	 int y = big;
	 int i = -1;
	 K::Point_2 p(x,y);
	 pts.push_back(p);
	 ptoi[p] = i;
      }
      {
	 int x = -big;
	 int y = big;
	 int i = -1;
	 K::Point_2 p(x,y);
	 pts.push_back(p);
	 ptoi[p] = i;
      }
      {
	 int x = big;
	 int y = -big;
	 int i = -1;
	 K::Point_2 p(x,y);
	 pts.push_back(p);
	 ptoi[p] = i;
      }
      {
	 int x = -big;
	 int y = -big;
	 int i = -1;
	 K::Point_2 p(x,y);
	 pts.push_back(p);
	 ptoi[p] = i;
      }
      Triangulation t;
      t.insert(pts.begin(), pts.end());

      for (Face_iterator f = t.finite_faces_begin(); f != t.finite_faces_end(); ++f) {
	 std::vector<int> vv;
	 vv.reserve(3);
	 for(int i=0; i<3; i++) {
	    if(!t.is_infinite(f->vertex(i))){
	       if(ptoi[f->vertex(i)->point()]>-1) {
	          vv.push_back(ptoi[f->vertex(i)->point()]);
	       }
	    }
	 }
	 for(int i : vv) {
	    for(int j : vv) {
	       if(i < j) {
	          Edge e(i,j);
		  long dx = xx[i] - xx[j];
		  long dy = yy[i] - yy[j];
		  long dd = dx*dx + dy*dy;
		  edges[e] = dd;
	       }
	    }
	 }
      }
      
      std::map<Edge,long> mstEdges;
      weighted_graph G(n);
      weight_map weights = boost::get(boost::edge_weight, G);
      edge_desc e;

      for(auto &it : edges) {
         const Edge &ee = it.first;
	 const long dd = it.second;
         e = boost::add_edge(ee.u,ee.v, G).first; weights[e]=dd;
	 //std::cout << "e: " << e.u << " " << e.v << " " << dd << std::endl;
      }
      std::vector<edge_desc> mst;
      boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));

      for (std::vector<edge_desc>::iterator it = mst.begin(); it != mst.end(); ++it) {
	 int i = boost::source(*it, G);
	 int j = boost::target(*it, G);
	 if(i>j) {std::swap(i,j);}
         Edge e(i,j);
	 mstEdges[e] = edges[e];
      }
      std::swap(edges,mstEdges);

      std::vector<int> comp(n);
      compComponents(n, p, edges, comp);
      //for(int i=0; i<n; i++) {
      //   std::cout << comp[i] << " ";
      //}
      //std::cout << std::endl;
      

      std::vector<Mission> ma;
      std::vector<Mission> mb;
      ma.reserve(m);
      mb.reserve(m);
      for(int i=0; i<m; i++) {
         int x0,y0,x1,y1;
	 std::cin >> x0 >> y0 >> x1 >> y1;

	 K::Point_2 p0(x0,y0);
	 K::Point_2 p1(x1,y1);
	 K::Point_2 &n0 = t.nearest_vertex(p0)->point();
	 K::Point_2 &n1 = t.nearest_vertex(p1)->point();
	 int i0 = ptoi[n0];
	 int i1 = ptoi[n1];
	 long dx0 = x0 - xx[i0];
	 long dx1 = x1 - xx[i1];
	 long dy0 = y0 - yy[i0];
	 long dy1 = y1 - yy[i1];
	 long dd = std::max(dx0*dx0 + dy0*dy0, dx1*dx1 + dy1*dy1) * 4;
	 //std::cout << dd << " " << p << std::endl;
	 if(dd <= p
	    and comp[i0]==comp[i1]) {
            std::cout << "y";
	    mb.push_back(Mission(i0,i1,dd));
	    ma.push_back(Mission(i0,i1,dd));
	 } else {
            std::cout << "n";
	    ma.push_back(Mission(i0,i1,dd));
	 }
      }
      std::cout << std::endl;
      std::cout << findMinEnergy(n,edges,comp,ma) << std::endl;
      std::cout << findMinEnergy(n,edges,comp,mb) << std::endl;
   }
}



