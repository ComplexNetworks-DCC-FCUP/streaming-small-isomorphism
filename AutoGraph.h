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
    bool** adjM;
  };

  struct AEdge
  {
    ANode* dest;
    int p1, p2;
  };

  void toggle(int a, int b);
  bool isConnected(int a, int b);
  string canonicalLabel();

  static int indexPair(int a, int b, int n);

private:
  void addEdge(int a, int b);
  void remEdge(int a, int b);

  int find(int a);
  string runNauty();
  void createNeighbor(ANode* cur, int a, int b);

  map<string, ANode*> graphMap;

  int* permutation;
  char* stmp;
  bool** adjM;
  ANode* cur;
  Isomorphism* iso;
};
