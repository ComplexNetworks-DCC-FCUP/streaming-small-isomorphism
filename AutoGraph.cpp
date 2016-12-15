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
  stmp2 = new char[n_nodes * n_nodes];

  adjM = new bool*[n_nodes];
  for (int i = 0; i < n_nodes; i++)
    adjM[i] = new bool[n_nodes];

  tmp = new int[n_nodes];
  tmp2 = new int[n_nodes];
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

  delete[] stmp;
  delete[] stmp2;
  delete[] permutation;
  delete[] tmp;
  delete[] tmp2;
  iso->finishNauty();
  delete iso;
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

string AutoGraph::runNauty()
{
  for (int i = 0; i < n_nodes; i++)
    for (int j = 0; j < n_nodes; j++)
      stmp[permutation[i] * n_nodes + permutation[j]] = adjM[i][j] ? '1' : '0';
  stmp[n_nodes * n_nodes] = '\0';

  iso->canonicalStrNauty(stmp, stmp2, tmp, tmp2);
  string sv = stmp2;

  return sv;
}

void AutoGraph::createNeighbor(ANode* cur, int a, int b)
{
  string s = runNauty();
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

  int ai = nperm[a], bi = nperm[b];

  cur->nei[indexPair(a, b)] = {n, nperm};
  n->nei[indexPair(ai, bi)] = {cur, npermi};

  for (int i = 0; i < n_nodes; i++)
    tmp[nperm[i]] = tmp2[i];

  compose(nperm);

  printf("P:");
  for (int i = 0; i < n_nodes; i++)
    printf(" %d", npermi[i]);
  printf(", %s\n", s.c_str());
  printf("O:");
  for (int i = 0; i < n_nodes; i++)
    printf(" %d", tmp[i]);
  printf(", %d %d (%d %d)\n", ai, bi, tmp[ai], tmp[bi]);

  for (int i = 0; i < n_nodes; i++)
    if (tmp[i] == tmp[ai])
      for (int j = (tmp[ai] == tmp[bi] ? i + 1 : 0); j < n_nodes; j++)
        if (tmp[j] == tmp[bi] && (i != ai || j != bi))
        {
          if (min(ai, bi) == min(i, j) && max(ai, bi) == max(i, j))
            continue;

          if (adjM[find(permutation, ai)][find(permutation, bi)] !=
              adjM[find(permutation, i)][find(permutation, j)])
            continue;

          for (int i = 0; i < n_nodes; i++)
            tmp2[i] = i;
          applyTranspositions(tmp2, ai, bi, i, j);

          int* npermorb = new int[n_nodes];
          for (int i = 0; i < n_nodes; i++)
            npermorb[i] = nperm[tmp2[i]];

          //cur->nei[indexPair(i, j)] = {n, npermorb};
          n->nei[indexPair(i, j)] = {cur, npermorb};
          printf("Other auto: %d %d <-> %d %d (%p)\nx:", i, j, ai, bi, npermorb);
          for (int i = 0; i < n_nodes; i++)
            printf(" %d", npermorb[i]);
          printf("\n");
        }
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
  else
  {
    compose(e.p);
    printf("%p ", e.p);
  }
  cur = e.dest;

  printf("p:");
  for (int i = 0; i < n_nodes; i++)
    printf(" %d", permutation[i]);
  printf("\n");
}

void AutoGraph::applyTranspositions(int* p, int a1, int a2, int b1, int b2)
{
  if (a1 > a2)
    swap(a1, a2);

  if (b1 > b2)
    swap(b1, b2);

  if (a2 == b1)
    swap(p[find(p, a1)],
         p[find(p, b2)]);
  else if (a1 == b2)
    swap(p[find(p, a2)],
         p[find(p, b1)]);
  else
  {
    swap(p[find(p, a1)],
         p[find(p, b1)]);
    swap(p[find(p, a2)],
         p[find(p, b2)]);
  }
}

int AutoGraph::find(int* p, int a)
{
  for (int i = 0; i < n_nodes; i++)
    if (p[i] == a)
      return i;
  return -1;
}

void AutoGraph::compose(int* perm)
{
  for (int i = 0; i < n_nodes; i++)
    tmp2[i] = perm[permutation[i]];
  for (int i = 0; i < n_nodes; i++)
    permutation[i] = tmp2[i];
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
