#pragma once

#include "IsoGraph.h"
#include "Isomorphism.h"
#include <map>

#define getPerm(p,i) (((p) >> ((i) * 4)) & 15)
#define setPerm(p,i,s) ((p) = (((p) & (~(15LL << ((i) * 4)))) | ((0LL + (s)) << ((i) * 4))))
typedef long long int Perm;

class AutoGraph : public IsoGraph
{
public:
  AutoGraph(bool _directed, int _n);
  ~AutoGraph();

  struct AEdge;
  struct ANode;

  struct ANode
  {
    string label;
    AEdge* nei;
  };

  struct AEdge
  {
    ANode* dest;
    Perm p;
  };

  void toggle(int a, int b);
  bool isConnected(int a, int b);
  string canonicalLabel();

private:
  void addEdge(int a, int b);
  void remEdge(int a, int b);

  void compose(Perm perm);
  Perm compress(Perm perm);

  int indexPair(int a, int b);
  void applyTranspositions(Perm p, int a1, int a2, int b1, int b2);

  string runNauty();
  void applyAutomatomChange(int a, int b);
  void createNeighbor(ANode* cur, int a, int b);

  map<string, ANode*> graphMap;

  Perm pers[12] = {0, 0, 16, 528, 12816, 274960, 5517840, 106181136LL, 1985229328LL, 36344967696LL, 654820258320LL, 11649936536080LL};
  int nei_size, step;
  Perm permutation, ipermutation, tmp, tmp2;
  int* tmpNauty, *tmp2Nauty;
  char* stmp, *stmp2;
  bool** adjM;
  ANode* cur;
  Isomorphism* iso;
};
