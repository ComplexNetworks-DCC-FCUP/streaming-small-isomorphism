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
  struct APerm;

  struct APerm
  {
    int *perm, p1, p2;
  };

  struct ANode
  {
    string label;
    AEdge* nei;
    map<string, APerm> edgePerm;
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
  void compose(int* perm);
  void applyTranspositions(int a, int b, int p1, int p2);

  string runNauty(bool apply = false);
  bool nautyCon(string& s, int i, int j);
  ANode* createNeighbor(ANode* cur, int a, int b);

  map<string, ANode*> graphMap;

  int* permutation, *tmp;
  char* stmp;
  bool** adjM;
  ANode* cur;
  Isomorphism* iso;
};
