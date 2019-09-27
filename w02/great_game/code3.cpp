// try with map instead of vectors, gives wrong answer answer, but times out anyway for test4

#include <iostream>
#include <vector>
#include <functional>
#include <map>

const std::vector<bool> player = {false,true,false,true};
const std::vector<bool> color = {false,true,true,false};
struct MyKey {
   int s,i,j;
   MyKey(int s,int i,int j) : s(s),i(i),j(j) {};
};
bool operator < (const MyKey& e1, const MyKey& e2) {
   return (e1.j<e2.j) or (e1.j==e2.j and (e1.i<e2.i or (e1.i==e1.i and e1.s<e2.s)));
}
int main() {
   std::ios_base::sync_with_stdio(false);
   

   int t(0);
   std::cin >> t;

   for(int tt=0;tt<t;tt++) {
      int n(0), m(0), r(0), b(0);
      std::cin >> n >> m >> r >> b;
      std::vector<std::vector<int>> transition;
      transition.resize(n);
      for(int mm=0; mm<m; mm++) {
         int u(0),v(0);
	 std::cin >> u >> v;
         transition[u-1].push_back(v-1);
      }
      
      std::map<MyKey,bool> table;
      
      std::function<bool(int,int,int)> f;
      f = [&table,&transition,&f,n](int s,int i,int j)->bool{
         if(i>=n-1) {return false;}
         if(j>=n-1) {return true;}
	 auto it = table.find(MyKey(s,i,j));
	 if(it==table.end()) {
 	    const int nextState = (s+1)%4;
            const bool p = player[s];
            const bool c = color[s];
	    
            if(c) {
	       // move j
	       for(auto jj : transition[j]) {
	          if(f(nextState,i,jj)==p) {
	             table.insert(std::make_pair(MyKey(s,i,j),p));
		     return p;
		  }
	       }
	    }else{
	       // move i
	       for(auto ii : transition[i]) {
	          if(f(nextState,ii,j)==p) {
	             table.insert(std::make_pair(MyKey(s,i,j),p));
		     return p;
	          }
	       }
	    }
            
	    table.insert(std::make_pair(MyKey(s,i,j),not p));
	    return not p;
	 }
      };
      
      std::cout << f(0,r-1,b-1) << std::endl;
   }
}
