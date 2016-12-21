#pragma once

#include <string>

using namespace std;

class IsoGraph
{
public:
  virtual void toggle(int a, int b) = 0;
  virtual bool isConnected(int a, int b) = 0;
  virtual string canonicalLabel() = 0;

  int* stat;

protected:
  bool directed;
  int n_nodes;

  virtual void addEdge(int a, int b) = 0;
  virtual void remEdge(int a, int b) = 0;
};
