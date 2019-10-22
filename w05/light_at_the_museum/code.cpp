#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <climits>
#include <queue>

// generate M-vectors: lights on per room, count how many possibilities exist
// max 30*100 lights on per room
// do this for 2^N (via split and list)
// find match, count



typedef std::priority_queue<int,std::vector<int>,std::greater<int>> PQ;

struct Element {
   int lights[30];
   Element(int *l) {
      for(int i=0;i<30;i++) {lights[i]=l[i];}
   };
   Element() {
      for(int i=0;i<30;i++) {lights[i]=0;}
   }
   Element(Element const &e) {
      for(int i=0;i<30;i++) {lights[i]=e.lights[i];}
   }
   Element(Element const &e,int *l) {
      for(int i=0;i<30;i++) {lights[i]=e.lights[i]+l[i];}
   }
   Element(Element const &e,int *l,bool bogus) {// get inverse: l-e
      for(int i=0;i<30;i++) {lights[i]=l[i]-e.lights[i];}
   }
};

bool operator < (const Element& e1, const Element& e2) {
   for(int i=0; i<30;i++){
      if(e1.lights[i]<e2.lights[i]) {return true;}
      if(e1.lights[i]>e2.lights[i]) {return false;}
   }
   return false;
}

void generateMap(std::map<Element,int> &map, std::map<Element,int> &mapSwap, int n, int m, int nStart, int nEnd, std::vector<int> &switches) {
   map.clear();
   mapSwap.clear();
   
   map[Element()] = 0;
   for(int i=nStart;i<nEnd;i++) {
      std::swap(map,mapSwap);
      map.clear();
      for(auto it = mapSwap.begin(); it!=mapSwap.end(); it++) {
         const Element &e = it->first;
         int count = it->second;
         Element eLeave(Element(e,&switches[i*2*30]));
         Element eSwitch(Element(e,&switches[i*2*30 + 30]));
         if(map.find(eLeave)==map.end()) {
            map[eLeave] = count;
         } else {
            map[eLeave] = std::min(map[eLeave],count);
         }
         if(map.find(eSwitch)==map.end()) {
            map[eSwitch] = count+1;
         } else {
            map[eSwitch] = std::min(map[eSwitch],count+1);
         }
      }
   }
}

int main () {
   std::ios_base::sync_with_stdio(false);
   
   int t(0);
   std::cin >> t;
   
   for(int tt=0; tt<t; tt++) {
      int n(0); int m(0);
      std::cin >> n >> m;
      
      std::vector<int> target(30,0);
      for(int i=0;i<m;i++) {std::cin >> target[i];}

      std::vector<int> switches(2*30*n,0);
      for(int i=0;i<n;i++) {// for each switch
         for(int j=0;j<m;j++) {// for each room
            std::cin >> switches[i*2*30 + 0*30 + j] >> switches[i*2*30 + 1*30 + j];
         }
      }
      
      std::map<Element,int> map1;
      std::map<Element,int> map2;
      std::map<Element,int> mapSwap;
      
      //generateMap(map1,mapSwap,n,m,0,n,switches);
      //Element eTarget(&target[0]);
      //if(map1.find(eTarget) != map1.end()) {
      //   std::cout << map1[eTarget] << std::endl;
      //} else {
      //   std::cout << "impossible" << std::endl;
      //}
      
      int minSwitches = n*2;
      generateMap(map1,mapSwap,n,m,0,n/2,switches);
      generateMap(map2,mapSwap,n,m,n/2,n,switches);
      for(auto it = map1.begin(); it!=map1.end(); it++) {
         const Element &e = it->first;
         int count = it->second;
         Element eMatch(Element(e,&target[0],true));
         if(map2.find(eMatch) != map2.end()) {
            minSwitches = std::min(minSwitches, count+map2[eMatch]);
         }
      }
      if(minSwitches < n*2) {
         std::cout << minSwitches << std::endl;
      } else {
         std::cout << "impossible" << std::endl;
      }
   }
}
