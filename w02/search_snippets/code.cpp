
#include <iostream>
#include <queue>
#include <climits>

// Idea: linear scan, using minHeap, take minimum distance you can find.


struct Entry {
   int iOfn;
   int pos;
   Entry(int iOfn, int pos) : iOfn(iOfn), pos(pos) {}
};
bool operator < (const Entry& e1, const Entry& e2) {return (e1.pos > e2.pos);}
 

int main() {
   std::ios_base::sync_with_stdio(false);

   int t(0);
   std::cin >> t;
   
   std::vector<std::vector<int>> p;
   std::vector<int> m;
   std::vector<int> index;
   for(int tt = 0; tt<t; tt++) {
      int n(0);
      std::cin >> n;
      p.resize(n);
      m.resize(n);
      index.resize(n);
      for(int i=0; i<n; i++) {
         std::cin >> m[i];
         p[i].resize(m[i]);
         index[i]=0;
      }
      for(int i=0; i<n; i++) {
         for(int j=0; j<m[i]; j++) {
            std::cin >> p[i][j];
         }
      }
      
      std::priority_queue<Entry> minHeap;
      int maxVal(0);

      for(int i=0; i<n; i++) {
         int pp(p[i][0]);
         minHeap.push(Entry(i,pp));
         maxVal = (maxVal<pp)?pp:maxVal;
      }
      
      int result = INT_MAX;
      
      while(true) {
         Entry top = minHeap.top();
         minHeap.pop();
         
         int newRes(maxVal-top.pos);
         result = (result>newRes)?newRes:result;
         
         int i(top.iOfn);
         
         if(index[i]+1 >= m[i]) {
            break;
         }
         
         index[i]++;
         int pp(p[i][index[i]]);
         minHeap.push(Entry(i,pp));
         maxVal = (maxVal<pp)?pp:maxVal;
         
      }
      std::cout << result+1 << std::endl;
   }
}


