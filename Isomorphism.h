#ifndef _ISOMORPHISM_
#define _ISOMORPHISM_

#define MAXS 200

#define MAXMAT 10000

//#include "Graph.h"

#define MAXN 20
#define WORKSPACE_SIZE MAXN*160

#include "nauty/tnauty.h"
#include <string>

using namespace std;

class Isomorphism {
 private:  
  bool dir;
  setword workspace[WORKSPACE_SIZE];
  int n,m, lab[MAXN], ptn[MAXN], orbits[MAXN];
  set *gv;
  graphnau g[MAXN*MAXM];
    
 public:
  void initNauty(int size, bool dir);
  void finishNauty();

  void canonicalStrNauty(string v, char *s, int *perm = NULL);
};

#endif

