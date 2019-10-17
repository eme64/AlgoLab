#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <climits>

struct Element {
   long x1,x2,x3,x4;
   Element(long x1,long x2,long x3,long x4)
      : x1(x1),x2(x2),x3(x3),x4(x4){};
   Element(const Element &el,long x1,long x2,long x3,long x4)
      : x1(el.x1+x1),x2(el.x2+x2),x3(el.x3+x3),x4(el.x4+x4){};
   bool isLegal(long side) {
      return x1<=side and x2<=side and x3<=side and x4<=side;
   }
};

bool operator < (const Element& e1, const Element& e2) {
   return (e1.x1 < e2.x1) or (e1.x1==e2.x1 and (
          (e1.x2 < e2.x2) or (e1.x2==e2.x2 and (
          (e1.x3 < e2.x3) or (e1.x3==e2.x3 and (
          (e1.x4<e2.x4) ))))));
}

int main () {
   std::ios_base::sync_with_stdio(false);
   
   int t(0);
   std::cin >> t;
   
   for(int tt=0; tt<t; tt++) {
      int n(0);
      std::cin >> n;
      
      std::vector<long> plank(n);
      long sum(0);
      for(int i=0; i<n; i++) {
         std::cin >> plank[i];
         sum+=plank[i];
      }
      if((sum%4)!=0) {
         std::cout << "0" << std::endl;
         continue;
      }
      long side(sum/4);
      
      std::sort(plank.begin(),plank.end());
     
      std::map<Element,long> table;
      std::map<Element,long> newTable;
      
      table[Element(0,0,0,0)] = 1;
      
      for(int i=n-1;i>=0;i--) {
         const long p = plank[i];
         newTable.clear();
         for(auto it = table.begin(); it!=table.end();it++) {
            const Element &el = it->first;
            long count = it->second;
            const Element el_1(el,p,0,0,0);
            const Element el_2(el,0,p,0,0);
            const Element el_3(el,0,0,p,0);
            const Element el_4(el,0,0,0,p);
            if(newTable.find(el_1) == table.end()) {newTable[el_1]=count;} else {newTable[el_1]+=count;}
            if(newTable.find(el_2) == table.end()) {newTable[el_2]=count;} else {newTable[el_2]+=count;}
            if(newTable.find(el_3) == table.end()) {newTable[el_3]=count;} else {newTable[el_3]+=count;}
            if(newTable.find(el_4) == table.end()) {newTable[el_4]=count;} else {newTable[el_4]+=count;}
         }
         //std::cout << plank[i] << std::endl;
         std::swap(newTable,table);
      }
      const Element el_final(side,side,side,side);
      if(table.find(el_final)!=table.end()) {
         std::cout << table[el_final]/24 << std::endl;
      } else {
         std::cout << 0 << std::endl;
      }
   }
}
