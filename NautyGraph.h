#pragma once

#include "IsoGraph.h"
#include "Isomorphism.h"

class NautyGraph : public IsoGraph
{
public:
  NautyGraph(bool _directed, int _n);
  ~NautyGraph();

  void toggle(int a, int b);
  bool isConnected(int a, int b);
  string canonicalLabel();

private:
  void addEdge(int a, int b);
  void remEdge(int a, int b);

  bool** adjM;
  char* stmp, *stmp2;
  Isomorphism* iso;
};
