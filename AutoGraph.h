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

  string runNauty(bool apply = false);
  void applyAutomatomChange(int a, int b);
  void createNeighbor(ANode* cur, int a, int b);

  map<string, ANode*> graphMap;

  int nei_size;
  int* permutation, *tmp;
  char* stmp;
  bool** adjM;
  ANode* cur;
  Isomorphism* iso;
};
