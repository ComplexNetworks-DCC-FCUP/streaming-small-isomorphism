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

  tmp = new int[n_nodes];
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

  iso->canonicalStrNauty(sv, stmp, tmp);
  sv = stmp;

  return sv;
}

bool AutoGraph::nautyCon(string& s, int i, int j)
{
  return s[i * n_nodes + j] == '1';
}

AutoGraph::ANode* AutoGraph::createNeighbor(ANode* cur, int a, int b)
{
  string s = runNauty(true);
  AutoGraph::ANode *n;
  bool apply;

  if (graphMap.count(s) != 0)
  {
    n = graphMap[s];
    apply = true;

    //printf("Created another edge from %s to %s, ", cur->label.c_str(), s.c_str());
  }
  else
  {
    n = new AutoGraph::ANode();

    n->nei = new AEdge[n_nodes * (n_nodes + 1) / 2];
    n->label = s;

    graphMap[s] = n;
    apply = false;

    //printf("Created new edge from %s to %s, ", cur->label.c_str(), s.c_str());
  }

  if (cur->edgePerm.count(s) == 0)
  {
    int* nperm1 = new int[n_nodes];

    for (int i = 0; i < n_nodes; i++)
      nperm1[tmp[i]] = i;

    cur->edgePerm[s] = {nperm1, a, b};

    /*printf("CC:");
    for (int i = 0; i < n_nodes; i++)
      printf(" %d", tmp[i]);
      printf(", ");*/
  }

/*  int f1 = -1, f2, f3, f4, p1, p2;

  for (int i = 0; i < n_nodes; i++)
    for (int j = i + 1; j < n_nodes; j++)
    {
      if (adjM[i][j] != nautyCon(n->label, i, j))
      {
        if (f1 == -1)
          f1 = i, f2 = j;
        else
          f3 = i, f4 = j;
      }
    }

  for (int i = 0; i < n_nodes; i++)
    for (int j = i + 1; j < n_nodes; j++)
    {
      if (adjM[i][j] != nautyCon(cur->label, i, j))
        p1 = i, p2 = j;
    }

  if (f1 != -1 && (p1 != f1 || p2 != f2))
    cur->nei[indexPair(p1, p2, n_nodes)] = {n, f1, f2};
  else if (f1 != -1 && (p1 != f3 || p2 != f4))
    cur->nei[indexPair(p1, p2, n_nodes)] = {n, f3, f4};
  else
  cur->nei[indexPair(p1, p2, n_nodes)] = {n, p1, p2};*/
  cur->nei[indexPair(a, b, n_nodes)] = {n, cur->edgePerm[s].p1, cur->edgePerm[s].p2};


//  if (apply)
//  {
    applyTranspositions(a, b, cur->nei[indexPair(a, b, n_nodes)].p1, cur->nei[indexPair(a, b, n_nodes)].p2);
    /*printf("CC:");
    for (int i = 0; i < n_nodes; i++)
      printf(" %d", permutation[i]);
      printf(", ");*/
    compose(cur->edgePerm[s].perm);
//  }
//  else
//  {
//    runNauty();
//    for (int i = 0; i < n_nodes; i++)
//      permutation[tmp[i]] = i;
//  }

    //printf("%d %d, %d %d %p\n", a, b, cur->nei[indexPair(a, b, n_nodes)].p1, cur->nei[indexPair(a, b, n_nodes)].p2, n);

  return n;
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
    cur = createNeighbor(cur, a, b);
  else
  {
    //printf("A, Edge exists! (%d, %d)\n", a, b);

    applyTranspositions(a, b, e.p1, e.p2);
    compose(cur->edgePerm[e.dest->label].perm);
    cur = e.dest;
  }

  /*printf("A: ");
  for (int i = 0; i < n_nodes; i++)
    for (int j = 0; j < n_nodes; j++)
      printf("%d", adjM[i][j]);
  printf(" ");

  int t[n_nodes][n_nodes];
  for (int i = 0; i < n_nodes; i++)
    for (int j = 0; j < n_nodes; j++)
      t[permutation[i]][permutation[j]] = adjM[i][j];
  for (int i = 0; i < n_nodes; i++)
    for (int j = 0; j < n_nodes; j++)
      printf("%d", t[i][j]);

  for (int i = 0; i < n_nodes; i++)
    printf(" %d", permutation[i]);
    printf("\n");*/
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
    cur = createNeighbor(cur, a, b);
  else
  {
//    printf("R, Edge exists! (%d, %d) -> (%d, %d) %p\n", a, b, e.p1, e.p2, e.dest);

    applyTranspositions(a, b, e.p1, e.p2);
    compose(cur->edgePerm[e.dest->label].perm);
    cur = e.dest;
  }

/*  printf("R: ");
  for (int i = 0; i < n_nodes; i++)
    for (int j = 0; j < n_nodes; j++)
      printf("%d", adjM[i][j]);
  printf(" ");

  int t[n_nodes][n_nodes];
  for (int i = 0; i < n_nodes; i++)
    for (int j = 0; j < n_nodes; j++)
      t[permutation[i]][permutation[j]] = adjM[i][j];
  for (int i = 0; i < n_nodes; i++)
    for (int j = 0; j < n_nodes; j++)
      printf("%d", t[i][j]);

  for (int i = 0; i < n_nodes; i++)
    printf(" %d", permutation[i]);
    printf("\n");*/
}

void AutoGraph::compose(int* perm)
{
  for (int i = 0; i < n_nodes; i++)
//    tmp[i] = permutation[perm[i]];
    tmp[i] = perm[permutation[i]];
  for (int i = 0; i < n_nodes; i++)
    permutation[i] = tmp[i];
}

void AutoGraph::applyTranspositions(int a, int b, int p1, int p2)
{
  if (b == p1)
    swap(permutation[find(a)],
         permutation[find(p2)]);
  else if (a == p2)
    swap(permutation[find(b)],
         permutation[find(p1)]);
  else
  {
    swap(permutation[find(a)],
         permutation[find(p1)]);
    swap(permutation[find(b)],
         permutation[find(p2)]);
  }
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
