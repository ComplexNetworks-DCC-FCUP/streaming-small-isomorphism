#pragma once

#include "IsoGraph.h"
#include "Isomorphism.h"
#include <map>

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
    int* p;
  };

  void toggle(int a, int b);
  bool isConnected(int a, int b);
  string canonicalLabel();

private:
  void addEdge(int a, int b);
  void remEdge(int a, int b);

  void compose(int* perm);

  int indexPair(int a, int b);
  void applyTranspositions(int* p, int a1, int a2, int b1, int b2);
  int find(int* p, int a);

  string runNauty();
  void applyAutomatomChange(int a, int b);
  void createNeighbor(ANode* cur, int a, int b);

  map<string, ANode*> graphMap;

  int nei_size, step;
  int* permutation, *tmp, *tmp2;
  char* stmp, *stmp2;
  bool** adjM;
  ANode* cur;
  Isomorphism* iso;
};
