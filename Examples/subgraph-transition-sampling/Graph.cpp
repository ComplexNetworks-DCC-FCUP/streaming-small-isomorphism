#include "Graph.h"

Graph::Graph() { }
Graph::~Graph()
{
  delete[] nnei;

  for (int i = 0; i < n_nodes; i++)
  {
    delete[] nei[i];
    delete[] nei_t[i];
  }

  delete[] nei;
  delete[] nei_t;
}

void Graph::createGraph(int n, bool d)
{
  n_nodes = n;
  dir = d;

  for (int i = 0; i < n_nodes; i++)
  {
    vector<int> v, t;
    v_nei.push_back(v);
    v_nei_t.push_back(t);
  }
}

int Graph::numNodes()
{
  return n_nodes;
}

void Graph::addEdge(int a, int b, int t)
{
  v_nei[a].push_back(b);
  v_nei_t[a].push_back(t);
}

bool Graph::hasEdge(int a, int b)
{
  return find(v_nei[a].begin(), v_nei[a].end(), b) != v_nei[a].end();
}

void Graph::prepareGraph()
{
  nnei = new int[n_nodes];
  nei = new int*[n_nodes];
  nei_t = new int*[n_nodes];

  for (int i = 0; i < n_nodes; i++)
  {
    nnei[i] = v_nei[i].size();
    nei[i] = new int[nnei[i]];
    nei_t[i] = new int[nnei[i]];

    for (int j = 0; j < nnei[i]; j++)
    {
      nei[i][j] = v_nei[i][j];
      nei_t[i][j] = v_nei_t[i][j];
    }
  }
}

int* Graph::arrayNeighbours(int a)
{
  return nei[a];
}

int* Graph::arrayTimNeighbours(int a)
{
  return nei_t[a];
}

int Graph::arrayNumNeighbours(int a)
{
  return nnei[a];
}
