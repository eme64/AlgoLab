#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <climits>

// simple split and list approach.

struct Element {
   const long x1,x2,x3,x4;
   Element(long x1,long x2,long x3,long x4)
      : x1(x1),x2(x2),x3(x3),x4(x4){};
   Element(const Element &el,long x1,long x2,long x3,long x4)
      : x1(el.x1+x1),x2(el.x2+x2),x3(el.x3+x3),x4(el.x4+x4){};
   bool isLegal(long side) {
      return x1<=side and x2<=side and x3<=side and x4<=side;
   }
   Element getMatch(long side) const {
      return Element(side-x1,side-x2,side-x3,side-x4);
   }
};

bool operator < (const Element& e1, const Element& e2) {
   return (e1.x1 < e2.x1) or (e1.x1==e2.x1 and (
          (e1.x2 < e2.x2) or (e1.x2==e2.x2 and (
          (e1.x3 < e2.x3) or (e1.x3==e2.x3 and (
          (e1.x4<e2.x4) ))))));
}

void computeMap(
   const std::vector<long> &plank,
   const int iFrom,
   const int iTo,
   std::map<Element,long> &table,
   std::map<Element,long> &newTable,
   const long side) {
   
   table.clear();
   table[Element(0,0,0,0)] = 1;
   
   for(int i=iFrom;i>=iTo;i--) {
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
      std::swap(newTable,table);
   }
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
     
      std::map<Element,long> table_1;
      std::map<Element,long> table_2;
      std::map<Element,long> table_swap;
     
      // split two ranges:
      computeMap(plank,n-1,n/2,table_1,table_swap,side);
      computeMap(plank,n/2-1,0,table_2,table_swap,side);
      
      long totalCount(0);
      
      // meet in the middle:
      for(auto it = table_1.begin(); it!=table_1.end();it++) {
         const Element &e1(it->first);
         const long count(it->second);
         const Element e2(e1.getMatch(side));
         if(table_2.find(e2)!=table_2.end()) {
            totalCount+=count*table_2[e2];
         }
      }
      std::cout << totalCount/24 << std::endl;
   }
}
