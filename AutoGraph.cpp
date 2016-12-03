#include "AutoGraph.h"

AutoGraph::AutoGraph(bool _directed, int _n)
{
  directed = _directed;
  n_nodes = _n;

  iso = new Isomorphism();
  iso->initNauty(_n, _directed);
  stmp = new char[n_nodes * n_nodes];

  adjM = new bool*[n_nodes];
  for (int i = 0; i < n_nodes; i++)
    adjM[i] = new bool[n_nodes];

  permutation = new int[n_nodes];
  for (int i = 0; i < n_nodes; i++)
    permutation[i] = i;

  AutoGraph::ANode *e0, *e1, *e2, *e3;
  e0 = new AutoGraph::ANode();
/*  e1 = new AutoGraph::ANode();
  e2 = new AutoGraph::ANode();
  e3 = new AutoGraph::ANode();*/

  // Empty graph
  e0->label = "";
  for (int i = 0; i < n_nodes * n_nodes; i++)
    e0->label += "0";

  e0->nei = new AEdge[n_nodes * (n_nodes + 1) / 2];
  e0->adjM = new bool*[n_nodes];
  for (int i = 0; i < n_nodes; i++)
  {
    e0->adjM[i] = new bool[n_nodes];
    for (int j = 0; j < n_nodes; j++)
      e0->adjM[i][j] = false;
  }

  graphMap[e0->label] = e0;

/*  e0->nei[indexPair(0, 1, n_nodes)] = {e1, 0, 1};
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
  e3->nei[indexPair(1, 2, n_nodes)] = {e2, 1, 2};*/

  cur = e0;
}

AutoGraph::~AutoGraph()
{
  for (int i = 0; i < n_nodes; i++)
    delete[] adjM[i];
  delete[] adjM;

  iso->finishNauty();
}

void AutoGraph::toggle(int a, int b)
{
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

string AutoGraph::runNauty()
{
  string sv;

  for (int i = 0; i < n_nodes; i++)
    for (int j = 0; j < n_nodes; j++)
      stmp[i * n_nodes + j] = adjM[i][j] ? '1' : '0';
  stmp[n_nodes * n_nodes] = '\0';
  sv = stmp;

  iso->canonicalStrNauty(sv, stmp);
  sv = stmp;

  return sv;
}

void AutoGraph::createNeighbor(ANode* cur, int a, int b)
{
  string s = runNauty();

  if (graphMap.count(s) != 0)
  {
    AutoGraph::ANode *n = graphMap[s];

    int f1 = a, f2 = b;

    for (int i = 0; i < n_nodes; i++)
      for (int j = i + 1; j < n_nodes; j++)
      {
        int ii = permutation[i], jj = permutation[j];
        if (ii > jj)
          swap(ii, jj);

        if (adjM[ii][jj] != n->adjM[i][j])
        {
          if (ii != a || jj != b)
          {
            f1 = ii, f2 = jj;
            break;
          }
          printf("dif %d %d\n", ii, jj);
        }
      }

    cur->nei[indexPair(a, b, n_nodes)] = {n, f1, f2};

    printf("Created another edge from %s to %s (%d, %d) - (%d, %d)\n", cur->label.c_str(), s.c_str(), f1, f2, a, b);
  }
  else
  {
    AutoGraph::ANode *n;
    n = new AutoGraph::ANode();

    n->nei = new AEdge[n_nodes * (n_nodes + 1) / 2];
    n->adjM = new bool*[n_nodes];
    n->label = s;

    for (int i = 0; i < n_nodes; i++)
    {
      n->adjM[i] = new bool[n_nodes];
      for (int j = 0; j < n_nodes; j++)
        n->adjM[i][j] = adjM[permutation[i]][permutation[j]];
    }

    graphMap[s] = n;

    cur->nei[indexPair(a, b, n_nodes)] = {n, a, b};
    n->nei[indexPair(a, b, n_nodes)] = {cur, a, b};

    printf("Created new edge from %s to %s (%d, %d)\n", cur->label.c_str(), s.c_str(), a, b);
  }
}

void AutoGraph::addEdge(int a, int b)
{
  adjM[a][b] = true;
  if (!directed)
    adjM[b][a] = true;

  a = permutation[a];
  b = permutation[b];
  if (a > b)
    swap(a, b);

  AEdge e = cur->nei[indexPair(a, b, n_nodes)];

  if (e.dest == NULL)
  {
    createNeighbor(cur, a, b);
    e = cur->nei[indexPair(a, b, n_nodes)];
  }

  swap(permutation[find(a)],
       permutation[find(e.p1)]);

  if (b == e.p1)
    swap(permutation[find(a)],
         permutation[find(e.p2)]);
  else
    swap(permutation[find(b)],
         permutation[find(e.p2)]);

  cur = e.dest;

  printf("A: ");
  for (int i = 0; i < n_nodes; i++)
    for (int j = 0; j < n_nodes; j++)
      printf("%d", adjM[i][j]);
  for (int i = 0; i < n_nodes; i++)
    printf(" %d", permutation[i]);
  printf("\n");
}

void AutoGraph::remEdge(int a, int b)
{
  adjM[a][b] = false;
  if (!directed)
    adjM[b][a] = false;

  a = permutation[a];
  b = permutation[b];
  if (a > b)
    swap(a, b);

  AEdge e = cur->nei[indexPair(a, b, n_nodes)];

  if (e.dest == NULL)
  {
    createNeighbor(cur, a, b);
    e = cur->nei[indexPair(a, b, n_nodes)];
  }

  swap(permutation[find(a)],
       permutation[find(e.p1)]);

  if (b == e.p1)
    swap(permutation[find(a)],
         permutation[find(e.p2)]);
  else
    swap(permutation[find(b)],
         permutation[find(e.p2)]);

  cur = e.dest;

  printf("R: ");
  for (int i = 0; i < n_nodes; i++)
    for (int j = 0; j < n_nodes; j++)
      printf("%d", adjM[i][j]);
  for (int i = 0; i < n_nodes; i++)
    printf(" %d", permutation[i]);
  printf("\n");
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
