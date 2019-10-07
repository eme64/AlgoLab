#include <iostream>
#include <vector>
#include <climits>

int main() {
   std::ios_base::sync_with_stdio(false);
   
   int t(0);
   std::cin >> t;
   
   for(int tt = 0; tt<t; tt++) {
      int n(0);
      std::cin >> n;
      
      std::vector<int> table(3*n,-1);
      // i*3 + j
      // j = dist to next taken one
      // j=0 take i, children don't matter: 0/1/2
      // j=1 don't take i, take 0/1 from children, one must be 0
      // j=2 don't take i, take 0/1 from children only
      
      std::vector<std::vector<int>> stages(n);
      
      for(int ii=0; ii<n-1; ii++) {
         int i(0),j(0);
         std::cin >> i >> j;
         stages[i].push_back(j);
      }
      
      std::vector<int> order;
      order.push_back(0);
      for(int pos=0;pos<n;pos++) {
         int i(order[pos]);
         for(auto j : stages[i]) {order.push_back(j);}
      }
 
      std::vector<int> cost(n);
      for(int i=0; i<n; i++) {
         std::cin >> cost[i];
      }
      
      // in reverse order walk and calculate:
      for(int pos=n-1; pos>=0; pos--) {
         int i(order[pos]);
         
         if(stages[i].size()==0) {
            table[i*3 + 0] = cost[i];
            table[i*3 + 1] = INT_MAX; // invalid
            table[i*3 + 2] = 0; // invalid
         } else {
            int sumMin012(0);
            int sumMin01(0);
            int min01Diff(INT_MAX);
            
            for(auto j : stages[i]) {
               int min01(0);
               if( table[j*3 + 0] > table[j*3 + 1] ) {
                  min01 = table[j*3 + 1];
                  min01Diff = std::min( min01Diff , table[j*3 + 0] - table[j*3 + 1]);
               } else {
                  min01 = table[j*3 + 0];
                  min01Diff = 0; // not taking it is better
               }
               int min012(std::min( min01 , table[j*3 + 2] ));
               
               sumMin012 += min012;
               sumMin01 += min01;
            }
            
            table[i*3 + 0] = cost[i] + sumMin012;
            table[i*3 + 1] = min01Diff + sumMin01;
            table[i*3 + 2] = sumMin01;
         }
      }
      
      std::cout << std::min(table[0],table[1]) << std::endl;
   }
}




