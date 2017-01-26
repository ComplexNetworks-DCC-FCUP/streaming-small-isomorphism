#pragma once

#include <vector>
#include <algorithm>

using namespace std;

class Graph
{
public:

  Graph();
  ~Graph();

  void createGraph(int n, bool d);
  int numNodes();

  void addEdge(int a, int b, int t);
  bool hasEdge(int a, int b);

  void prepareGraph();

  int* arrayNeighbours(int a);
  int* arrayTimNeighbours(int a);
  int arrayNumNeighbours(int a);

private:
  vector<vector<int>> v_nei;
  vector<vector<int>> v_nei_t;

  int* nnei;
  int** nei;
  int** nei_t;

  int n_nodes;
  bool dir;
};
