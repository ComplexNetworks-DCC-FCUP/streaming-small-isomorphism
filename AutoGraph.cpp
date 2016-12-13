#include "AutoGraph.h"
#include <assert.h>

AutoGraph::AutoGraph(bool _directed, int _n)
{
  directed = _directed;
  n_nodes = _n;
  nei_size = directed ?
    n_nodes * n_nodes : 
    n_nodes * (n_nodes + 1) / 2;

  iso = new Isomorphism();
  iso->initNauty(_n, _directed);
  stmp = new char[n_nodes * n_nodes];

  adjM = new bool*[n_nodes];
  for (int i = 0; i < n_nodes; i++)
    adjM[i] = new bool[n_nodes];

  tmp = new int[n_nodes];
  permutation = new int[n_nodes];
  for (int i = 0; i < n_nodes; i++)
    permutation[i] = i;

  AutoGraph::ANode *e0 = new AutoGraph::ANode();

  // Empty graph
  e0->label = "";
  for (int i = 0; i < n_nodes * n_nodes; i++)
    e0->label += "0";

  e0->nei = new AEdge[nei_size];

  graphMap[e0->label] = e0;

  cur = e0;
}

AutoGraph::~AutoGraph()
{
  for (int i = 0; i < n_nodes; i++)
    delete[] adjM[i];
  delete[] adjM;

  delete[] permutation;
  delete[] tmp;
  iso->finishNauty();
}

void AutoGraph::toggle(int a, int b)
{
  if (isConnected(a, b))
    remEdge(a, b);
  else
    addEdge(a, b);

  applyAutomatomChange(a, b);
}

bool AutoGraph::isConnected(int a, int b)
{
  return directed ? (adjM[a][b]) : (adjM[a][b] || adjM[b][a]);
}

string AutoGraph::canonicalLabel()
{
  return cur->label;
}

string AutoGraph::runNauty(bool apply)
{
  string sv;

  for (int i = 0; i < n_nodes; i++)
    for (int j = 0; j < n_nodes; j++)
      if (apply)
        stmp[permutation[i] * n_nodes + permutation[j]] = adjM[i][j] ? '1' : '0';
      else
        stmp[i * n_nodes + j] = adjM[i][j] ? '1' : '0';
  stmp[n_nodes * n_nodes] = '\0';
  sv = stmp;

  iso->canonicalStrNauty(sv, stmp, tmp, tmp2);
  sv = stmp;

  return sv;
}

void AutoGraph::createNeighbor(ANode* cur, int a, int b)
{
  string s = runNauty(true);
  AutoGraph::ANode *n;

  if (graphMap.count(s) != 0)
    n = graphMap[s];
  else
  {
    n = new AutoGraph::ANode();

    n->nei = new AEdge[nei_size];
    n->label = s;

    graphMap[s] = n;
  }

  int* nperm = new int[n_nodes];
  int* npermi = new int[n_nodes];
  for (int i = 0; i < n_nodes; i++)
    nperm[tmp[i]] = i;
  memcpy(npermi, tmp, n_nodes * sizeof(int));

  cur->nei[indexPair(a, b)] = {n, nperm};
  n->nei[indexPair(nperm[a], nperm[b])] = {cur, npermi};
}

void AutoGraph::addEdge(int a, int b)
{
  adjM[a][b] = true;
  if (!directed)
    adjM[b][a] = true;
}

void AutoGraph::remEdge(int a, int b)
{
  adjM[a][b] = false;
  if (!directed)
    adjM[b][a] = false;
}

void AutoGraph::applyAutomatomChange(int a, int b)
{
  a = permutation[a];
  b = permutation[b];

  AEdge e = cur->nei[indexPair(a, b)];

  if (e.dest == NULL)
  {
    createNeighbor(cur, a, b);
    e = cur->nei[indexPair(a, b)];
  }

  compose(e.p);
  cur = e.dest;
}

void AutoGraph::compose(int* perm)
{
  for (int i = 0; i < n_nodes; i++)
    tmp[i] = perm[permutation[i]];
  for (int i = 0; i < n_nodes; i++)
    permutation[i] = tmp[i];
}

int AutoGraph::indexPair(int a, int b)
{
  if (!directed)
  {
    if (a > b)
      swap(a, b);
    return (2 * n_nodes - a - 3) * a / 2 + b - 1;
  }
  else
    return a * n_nodes + b;
}
