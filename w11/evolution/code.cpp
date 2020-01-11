#include <iostream>
#include <vector>
#include <map>
#include <cassert>

// possible improvement for strings: map to long?

struct Test {
   int n,q;
   std::map<std::string,int> name2i;
   std::vector<std::string> i2name;
   std::vector<int> i2age;
   std::vector<int> nChildren;
   std::vector<int> offset;
   std::vector<int> children;
   std::vector<int> parent;
   std::vector<int> logExp; // log explore anchestors
   Test() {
      std::cin >> n >> q;

      std::string name;
      int age;
      i2age.reserve(n);
      i2name.reserve(n);
      for(int i=0; i<n; i++) {
	 std::cin >> name >> age;
	 i2age.push_back(age);
	 name2i[name] = i;
	 i2name.push_back(name);
      }
      
      nChildren.resize(n,0);
      offset.resize(n,0);
      parent.resize(n,-1);
      std::string name2;
      for(int i=0; i<n-1; i++) {
         std::cin >> name >> name2;
	 int i1 = name2i[name];
	 int i2 = name2i[name2];
	 nChildren[i2]++;
	 parent[i1] = i2;
      }
      children.resize(n,-1);
      int sum = 0;// prefix sum for children offset
      for(int i=0; i<n; i++) {
         int o = nChildren[i];
	 offset[i] = sum;
	 sum+=o;
	 //std::cout << i2name[i] << " " << nChildren[i] << " " << offset[i] << std::endl;
      }
      std::vector<int> index(n,0);
      int luca = -1;
      for(int i=0; i<n; i++) {
         int p = parent[i];
	 if(p>=0) {
	    int oo = offset[p] + index[p]++;
	    children[oo] = i;
	 } else {
	    luca = i;
	 }
      }
      
      // set up logExp:
      logExp.resize(n*20,-1);

      // visit:
      std::vector<int> stack;
      std::vector<int> stackO;
      stack.resize(n,-1);
      stackO.resize(n,0);
      int stackI = 0;
      stack[0] = luca;
      stackO[0] = 0;

      while(stackI >= 0) {
	 int i = stack[stackI];
         // check if have more children to look at:
	 if(stackO[stackI]<nChildren[i]) {
            // visit child
	    int c = children[offset[i] + stackO[stackI]++];
	    //std::cout << stackI << " " << i2name[i] << " " << i2name[c] << std::endl;
            stackI++; // push
	    stack[stackI] = c;
	    stackO[stackI] = 0;

	    // write in logExp data:
	    int e = 1;
	    int ee = 0;
	    while(e<=stackI) {
	       logExp[c*20 + ee] = stack[stackI-e];
	       e = e << 1;
	       ee++;
	    }
	 } else {
	    // return one
	    //std::cout << stackI << " return " << i << " children: " << nChildren[i] << std::endl;
	    stackI--;
	 }
      }

      
      for(int i=0; i<q; i++) {
         std::cin >> name >> age;
	 int i1 = name2i[name];
         
	 //// correct but really slow
	 //int curr = i1;
	 //while(i2age[parent[curr]] <= age) {
	 //   curr = parent[curr];
	 //}
	 //std::cout << i2name[curr] << " ";

         
	 int curr = i1;
         
         while(curr!=luca and i2age[parent[curr]] <= age) {
            //std::cout << curr << " " << i2age[curr] << " " << age << std::endl;
	    int ee = 0;
	    while(logExp[curr*20 + ee+1]>=0 and i2age[logExp[curr*20 + ee+1]] <= age) {
	       ee++;
	       //std::cout << "  +" << ee << " " << logExp[curr*20 + ee] << " " << i2age[logExp[curr*20 + ee]] << std::endl;
	    }
            curr = logExp[curr*20 + ee];
	 }

	 //// correct but really slow
	 //int curr2 = i1;
	 //while(parent[curr2]>=0 and i2age[parent[curr2]] <= age) {
	 //   curr2 = parent[curr2];
	 //}

         //if(curr != curr2) {
	 //   std::cout << std::endl;
	 //   std::cout << "error: " << age << std::endl;
	 //   std::cout << curr << " " << i2age[curr] << std::endl;
	 //   std::cout << curr2 << " " << i2age[curr2] << std::endl;
	 //}
         //assert(curr == curr2);
         
	 if(i==0) {
	    std::cout << i2name[curr];
	 } else {
	    std::cout << " " << i2name[curr];
	 }
      }
      std::cout << std::endl;
   }
};


int main() {
   std::ios_base::sync_with_stdio(false);

   int t;
   std::cin >> t;
   for(int tt=0; tt<t; tt++) {
      Test ttt;
   }
}
