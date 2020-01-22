// idea:
// possible value states: 2*100*5000 = 10^6
// can we check feasibility?
// traverse table somehow?

#include <iostream>
#include <vector>
#include <cassert>

struct Canton {
   std::vector<bool> jobVsZone;
   std::vector<int> c;
   std::vector<int> p;
   int z,j;
   
   enum Decision : char {
      Yes = 0,
      No = 1,
      Unknown = 2,
   };

   Canton() {
      std::cin >> z >> j;

      //std::cout << "# z: " << z << " j: " << j << std::endl;
      c.resize(z);
      for(int i=0; i<z; i++) {
	 std::cin >> c[i];
      }

      p.resize(j);
      for(int i=0; i<j; i++) {
	 std::cin >> p[i];
      }
      
      jobVsZone.resize(z*j,false);
      for(int i=0; i<j; i++) {
         int ni;
	 std::cin >> ni;
	 for(int n=0; n<ni; n++) {
	    int zz;
	    std::cin >> zz;
	    jobVsZone[i*z+zz] = true;
	 }
      }
   }

   int run() {
      int costs = 0;
      int profits = 0;
      for(int &pp : p) {profits+=pp;}
      std::vector<bool> jobs(j,true);
      std::vector<Decision> decision(z,Unknown);
      std::vector<int> numJobs(z,0);
      for(int zz=0; zz<z; zz++) {
         for(int jj=0; jj<j; jj++) {
	    if(jobVsZone[jj*z+zz]) {numJobs[zz]++;}
	 }
      }
      

      return go(costs, profits, jobs, decision, numJobs);
   }

   int go(int costs, int profits,
          std::vector<bool> &jobs,
	  std::vector<Decision> &decision,
	  std::vector<int> &numJobs) {
      int maxJobs = 0;
      int maxZone = -1;
      for(int zz=0; zz<z; zz++) {
	 if(decision[zz]!=Unknown) {continue;}
	 if(numJobs[zz]==0) {decision[zz]=No;} // prune
         if(numJobs[zz]>=maxJobs) {maxJobs = numJobs[zz]; maxZone = zz;}
      }
      if(maxJobs==0) {
	 //std::cout << "no jobs left" << std::endl;
         return profits-costs;// terminate if no jobs left
      }
      if(maxJobs==1) {
	 //std::cout << "maxJobs==1" << std::endl;
         // handle individually:
	 std::vector<int> jobCosts(j,0);
	 for(int zz=0; zz<z; zz++) {
            if(decision[zz]!=Unknown) {continue;}
	    assert(numJobs[zz]==1);
            int jjj = 0;
	    while(!jobs[jjj] || !jobVsZone[jjj*z+zz]) {jjj++;}
            jobCosts[jjj]+= c[zz];
	    //std::cout << "zone " << zz << " for " << jjj << std::endl;
	 }
	 for(int jjj=0; jjj<j; jjj++) {
	    if(jobs[jjj]) {
	       if(jobCosts[jjj]<p[jjj]) {
	          costs+=jobCosts[jjj];
		  //std::cout << "take " << jjj << std::endl;
	       } else {
	          profits-=p[jjj];
		  //std::cout << "reject " << jjj << std::endl;
	       }
	    }
	 }
	 return profits-costs;
      }

      //std::cout << "# " << maxJobs << " " << maxZone << std::endl;
      // take zone:
      std::vector<Decision> decision1(z);
      for(int zz=0; zz<z; zz++) {decision1[zz] = decision[zz];}
      assert(decision1[maxZone]==Unknown);
      decision1[maxZone] = Yes;
      int c1 = go(costs+c[maxZone],profits,jobs,decision1,numJobs);
      
      // reject zone:
      decision1[maxZone] = No;
      int profit2 = profits;
      std::vector<bool> jobs2(j);
      std::vector<int> numJobs2(z);
      for(int zz=0; zz<z; zz++) {numJobs2[zz] = numJobs[zz];}
      for(int jjj=0; jjj<j; jjj++) {
	 jobs2[jjj] = jobs[jjj];
         if(jobs[jjj] && jobVsZone[jjj*z+maxZone]) {
	    // job was active and is now removed
	    jobs2[jjj] = false;
            profit2-=p[jjj];
            for(int zz=0; zz<z; zz++) {numJobs2[zz]-=(jobVsZone[jjj*z+zz]);}
	 }
      }
      int c2 = go(costs, profit2, jobs2, decision1, numJobs2);
      //std::cout << "c1: " << c1 << " c2: " << c2 << std::endl;
      return std::max(c1,c2);
   }
};


int main() {
   std::ios_base::sync_with_stdio(false);

   int t;
   std::cin >> t;
   for(int tt=0; tt<t; tt++) {
      Canton c;
      std::cout << c.run() << std::endl;
   }
}
