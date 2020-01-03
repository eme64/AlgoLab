#include <iostream>
#include <map>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<int,K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;

typedef Triangulation::Finite_faces_iterator Face_iterator;
typedef Triangulation::All_faces_iterator All_face_iterator;
typedef Triangulation::Face_handle Face;

struct InvLong {
   long value;
   InvLong() {}
   InvLong(long v) : value(v) {}
};
bool operator < (const InvLong& e1, const InvLong& e2) { return (e1.value > e2.value); }
bool operator > (const InvLong& e1, const InvLong& e2) { return (e1.value < e2.value); }
bool operator <= (const InvLong& e1, const InvLong& e2) { return (e1.value >= e2.value); }
bool operator >= (const InvLong& e1, const InvLong& e2) { return (e1.value <= e2.value); }

struct Edge {
   int u,v;
   Edge(int u, int v) : u(u),v(v) {}
};
bool operator < (const Edge& e1, const Edge& e2) {
   return (e1.u < e2.u or (e1.u == e2.u and e1.v < e2.v));
}

#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/adjacency_list.hpp>
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
boost::no_property, boost::property<boost::edge_weight_t, InvLong> > weighted_graph;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::out_edge_iterator out_edge_it;


int main() {
   std::ios_base::sync_with_stdio(false);
   
   while(true) {
      int n,m;
      std::cin >> n;
      if(n==0) {break;}
      
      std::vector<K::Point_2> pts;
      pts.reserve(n);
      std::map<K::Point_2,int> ptoi;
      std::vector<long> xx;
      std::vector<long> yy;
      xx.reserve(n);
      yy.reserve(n);
      for(int i=0; i<n; i++) {
         int x,y;
	 std::cin >> x >> y;
         pts.push_back(K::Point_2(x,y));
	 ptoi[pts[i]] = i;
	 xx.push_back(x);
	 yy.push_back(y);
      }
      Triangulation t;
      t.insert(pts.begin(),pts.end());
      
      // label faces:
      long big = 1l << 60;
      int cnt = 1;
      for (All_face_iterator f = t.all_faces_begin(); f != t.all_faces_end(); ++f) {
         //std::cout << t.triangle(f) << "\n";
         if(t.is_infinite(f)) {
	    f->info() = big;
	 } else {
	    f->info() = cnt++;
	 }
      }
      
      //std::cout << "cnt " << cnt << std::endl;
      weighted_graph G(cnt);
      weight_map weights = boost::get(boost::edge_weight, G);
      edge_desc e;

      std::vector<long> critical(cnt,0);
      for (All_face_iterator f = t.all_faces_begin(); f != t.all_faces_end(); ++f) {
         int i = f->info();
         //std::cout << t.triangle(f) << "\n";
	 if(t.is_infinite(f)) {
            critical[i] = big;
	 } else {
	    // add edges
	    for(int nn=0; nn<3; nn++) {
	       Face nei = f->neighbor(nn);
	       int ni = nei->info();
	       K::Point_2 &p0 = f->vertex((nn+1) % 3)->point();
	       K::Point_2 &p1 = f->vertex((nn+2) % 3)->point();
	       int i0 = ptoi[p0];
	       int i1 = ptoi[p1];
	       long dx = xx[i0] - xx[i1];
	       long dy = yy[i0] - yy[i1];
	       long dd = dx*dx+dy*dy;
	       //std::cout << "insert " << i << " " << ni << " " << dd << std::endl;
	       e = boost::add_edge(i, ni, G).first;
	       weights[e]=InvLong(dd);
	    }
	 }
      }
      
      weighted_graph G2(cnt);
      std::map<Edge,long> emap;

      std::vector<edge_desc> mst;
      boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));
      for (std::vector<edge_desc>::iterator it = mst.begin(); it != mst.end(); ++it) {
         int u = boost::source(*it, G);
	 int v = boost::target(*it, G);
	 //std::cout << boost::source(*it, G) << " " << boost::target(*it, G) << "\n";
	 e = boost::add_edge(u,v, G2).first;
	 long dd = weights[*it].value;
	 //std::cout << "mst: " << u << " " << v << " " << dd << std::endl;
	 emap[Edge(u,v)] = dd;
	 emap[Edge(v,u)] = dd;
      }

      std::vector<bool> visited(cnt,false);
      std::vector<int> queue;
      queue.reserve(n);
      int qIndex = 0;
      queue.push_back(0);
      visited[0] = true;
      
      while(queue.size() > qIndex) {
         int v = queue[qIndex++];
         
         out_edge_it oe_beg, oe_end;
	 for (boost::tie(oe_beg, oe_end) = boost::out_edges(v, G2); oe_beg != oe_end; ++oe_beg) {
            assert(boost::source(*oe_beg, G) == v);
	    int u = boost::target(*oe_beg, G);
            //std::cout << v << " " << u << " " << emap[Edge(u,v)] << std::endl;

	    if(!visited[u]) {
	       visited[u] = true;
	       queue.push_back(u);
               critical[u] = std::min(emap[Edge(u,v)], critical[v]);
	       //std::cout << "set: " << u << " " << v << " " << emap[Edge(u,v)] << " " << critical[v] << std::endl;
	    }
         }
      }


      // queries:
      std::cin >> m;
      for(int i=0; i<m; i++) {
         int x,y;
	 long d;
	 std::cin >> x >> y >> d;
	 K::Point_2 q(x,y);
	 Face f = t.locate(q);
	 //std::cout << "q" << i << " " << f->info() << std::endl;

	 // check that not too close to any point:
	 bool canStart = true;
	 for(int v=0; v<3; v++) {
	    auto vv = f->vertex(v);
	    if(!t.is_infinite(vv)) {
	       if(CGAL::squared_distance(vv->point(),q) < d) {
	          canStart = false;
	          //std::cout << "   " << vv->point() << " " << CGAL::squared_distance(vv->point(),q) << std::endl;
	       }
	    } else {
	       //std::cout << "inf-pass" << std::endl;
	    }
	 }
	 //std::cout << "start " << canStart << std::endl;
	 if(!canStart) {
	    std::cout << "n";
	    //std::cout << "n start " << x << " " << y << " " << d << std::endl;
            continue;
	 }

	 if(CGAL::squared_distance(t.nearest_vertex(q)->point(),q) < d) {
	    std::cout << "n";
	    //std::cout << "n point " << x << " " << y << " " << d << std::endl;
            continue;
	 }

	 // check if path out exists:
	 int ii = f->info();
	 //std::cout << "critical: " << critical[ii] << " " << d << std::endl;
	 if(critical[ii]>=d*4) {
	    std::cout << "y";
	    //std::cout << "y " << x << " " << y << " " << d << " " << critical[ii] << " " << t.is_infinite(f) << std::endl;
	    //if(t.is_infinite(f)) {
	    //   std::cout << "    "<< t.triangle(f) << std::endl;
	    //}
	 } else {
	    std::cout << "n";
	    //std::cout << "n " << x << " " << y << " " << d << std::endl;
	 }
      }
      std::cout << std::endl;
   }
}
