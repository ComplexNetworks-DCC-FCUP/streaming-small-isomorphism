#include "AutoGraph.h"

AutoGraph::AutoGraph(bool _directed, int _n)
{
  directed = _directed;
  n_nodes = _n;

  adjM = new bool*[n_nodes];
  for (int i = 0; i < n_nodes; i++)
    adjM[i] = new bool[n_nodes];

  permutation = new int[n_nodes];
  for (int i = 0; i < n_nodes; i++)
    permutation[i] = i;

  AutoGraph::ANode *e0, *e1, *e2, *e3;
  e0 = new AutoGraph::ANode();
  e1 = new AutoGraph::ANode();
  e2 = new AutoGraph::ANode();
  e3 = new AutoGraph::ANode();

  // Empty graph
  e0->label = "000000000";
  e0->nei = new AEdge[n_nodes * (n_nodes + 1) / 2];
  e0->nei[indexPair(0, 1, n_nodes)] = {e1, 0, 1};
  e0->nei[indexPair(0, 2, n_nodes)] = {e1, 0, 1};
  e0->nei[indexPair(1, 2, n_nodes)] = {e1, 0, 1};

  // One edge graph
  e1->label = "000001010";
  e1->nei = new AEdge[n_nodes * (n_nodes + 1) / 2];
  e1->nei[indexPair(0, 1, n_nodes)] = {e0, 0, 1};
  e1->nei[indexPair(0, 2, n_nodes)] = {e2, 0, 2};
  e1->nei[indexPair(1, 2, n_nodes)] = {e2, 0, 2};

  // ele graph
  e2->label = "001001110";
  e2->nei = new AEdge[n_nodes * (n_nodes + 1) / 2];
  e2->nei[indexPair(0, 1, n_nodes)] = {e1, 0, 2};
  e2->nei[indexPair(0, 2, n_nodes)] = {e1, 0, 2};
  e2->nei[indexPair(1, 2, n_nodes)] = {e3, 1, 2};

  // triangle graph
  e3->label = "011101110";
  e3->nei = new AEdge[n_nodes * (n_nodes + 1) / 2];
  e3->nei[indexPair(0, 1, n_nodes)] = {e2, 1, 2};
  e3->nei[indexPair(0, 2, n_nodes)] = {e2, 1, 2};
  e3->nei[indexPair(1, 2, n_nodes)] = {e2, 1, 2};

  cur = e0;
}

AutoGraph::~AutoGraph()
{
  for (int i = 0; i < n_nodes; i++)
    delete[] adjM[i];
  delete[] adjM;
}

void AutoGraph::toggle(int a, int b)
{
  a = permutation[a];
  b = permutation[b];

  if (a > b)
    swap(a, b);

  if (isConnected(a, b))
    remEdge(a, b);
  else
    addEdge(a, b);
}

bool AutoGraph::isConnected(int a, int b)
{
  return directed ? (adjM[a][b]) : (adjM[a][b] || adjM[b][a]);
}

string AutoGraph::canonicalLabel()
{
  return cur->label;
}

void AutoGraph::addEdge(int a, int b)
{
  adjM[a][b] = true;
  if (!directed)
    adjM[b][a] = true;

  swap(permutation[find(a)],
       permutation[find(cur->nei[indexPair(a, b, n_nodes)].p1)]);

  if (b == cur->nei[indexPair(a, b, n_nodes)].p1)
    swap(permutation[find(a)],
         permutation[find(cur->nei[indexPair(a, b, n_nodes)].p2)]);
  else
    swap(permutation[find(b)],
         permutation[find(cur->nei[indexPair(a, b, n_nodes)].p2)]);

  cur = cur->nei[indexPair(a, b, n_nodes)].dest;
}

void AutoGraph::remEdge(int a, int b)
{
  adjM[a][b] = false;
  if (!directed)
    adjM[b][a] = false;

  swap(permutation[find(a)],
       permutation[find(cur->nei[indexPair(a, b, n_nodes)].p1)]);

  if (b == cur->nei[indexPair(a, b, n_nodes)].p1)
    swap(permutation[find(a)],
         permutation[find(cur->nei[indexPair(a, b, n_nodes)].p2)]);
  else
    swap(permutation[find(b)],
         permutation[find(cur->nei[indexPair(a, b, n_nodes)].p2)]);

  cur = cur->nei[indexPair(a, b, n_nodes)].dest;
}

int AutoGraph::find(int a)
{
  for (int i = 0; i < n_nodes; i++)
    if (permutation[i] == a)
      return i;
  return -1;
}

int AutoGraph::indexPair(int a, int b, int n)
{
  return (2 * n - a - 3) * a / 2 + b - 1;
}
