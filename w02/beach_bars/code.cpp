#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
int main () {
   std::ios_base::sync_with_stdio(false);
   
   int t(0);
   std::cin >> t;
   
   std::vector<int> x;
   std::vector<int> solutions;
   for(int tt=0; tt<t; tt++) {
      int n(0);
      std::cin >> n;
      
      x.resize(n);
      
      for(int i=0; i<n; i++) {
         std::cin >> x[i];
      }
      
      std::sort(x.begin(),x.end());
      
      int maxCount = 0;
      int maxDistance = INT_MAX;
      solutions.reserve(n);
      solutions.clear();
      
      int i(0);
      int j(0);

      while(i<n) {
         // given an i, find maximal j
         while(j<n-1 and x[j+1]-x[i]<=200) {j++;}
         
         int count(j-i+1);
         int diameter(x[j]-x[i]);
         int distance((diameter+1)/2);

         if(count > maxCount) {
            maxCount = count;
            maxDistance = INT_MAX;
         }
         if(count == maxCount and maxDistance > distance) {
            maxDistance = distance;
            solutions.clear();
         }
         if(count == maxCount and distance==maxDistance) {
            if(diameter%2==1){
               solutions.push_back(x[i]+distance-1);
            }
            solutions.push_back(x[i]+distance);
         }
         
         i++;   
      }
      
      std::cout << maxCount << " " << maxDistance << std::endl;
      
      std::cout << solutions[0]; 
      for(int i=1; i<solutions.size(); i++) {
          std::cout << " " << solutions[i];
      }
      std::cout << std::endl;
   }
}
