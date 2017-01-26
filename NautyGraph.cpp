#include "NautyGraph.h"

NautyGraph::NautyGraph(bool _directed, int _n)
{
  directed = _directed;
  n_nodes = _n;

  adjM = new bool*[n_nodes];
  for (int i = 0; i < n_nodes; i++)
    adjM[i] = new bool[n_nodes];

  stmp = new char[n_nodes * n_nodes];
  stmp2 = new char[n_nodes * n_nodes];

  iso = new Isomorphism();
  iso->initNauty(n_nodes, directed);
}

NautyGraph::~NautyGraph()
{
  for (int i = 0; i < n_nodes; i++)
    delete[] adjM[i];
  delete[] adjM;
  delete[] stmp;
  delete[] stmp2;

  iso->finishNauty();
  delete iso;
}

void NautyGraph::toggle(int a, int b)
{
  if (isConnected(a, b))
    remEdge(a, b);
  else
    addEdge(a, b);
}

bool NautyGraph::isConnected(int a, int b)
{
  return directed ? (adjM[a][b]) : (adjM[a][b] || adjM[b][a]);
}

string NautyGraph::canonicalLabel()
{
  for (int i = 0; i < n_nodes; i++)
    for (int j = 0; j < n_nodes; j++)
      stmp[i * n_nodes + j] = adjM[i][j] ? '1' : '0';
  stmp[n_nodes * n_nodes] = '\0';

  iso->canonicalStrNauty(stmp, stmp2, NULL);
  string sv = stmp2;

  return sv;
}

void NautyGraph::cleanup()
{
  for (int i = 0; i < n_nodes; i++)
    for (int j = 0; j < n_nodes; j++)
      adjM[i][j] = 0;
}

void NautyGraph::addEdge(int a, int b)
{
  adjM[a][b] = true;
  if (!directed)
    adjM[b][a] = true;
}

void NautyGraph::remEdge(int a, int b)
{
  adjM[a][b] = false;
  if (!directed)
    adjM[b][a] = false;
}
