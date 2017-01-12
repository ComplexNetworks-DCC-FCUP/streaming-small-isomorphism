#include "AutoGraph.h"
#include <assert.h>

AutoGraph::AutoGraph(bool _directed, int _n, bool _prebuild)
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

  for (int i = 0; i < n_nodes; i++)
    for (int j = 0; j < n_nodes; j++)
      adjM[i][j] = 0;

  tmpNauty = new int[n_nodes];
  tmp2Nauty = new int[n_nodes];
  tmp = 0;
  tmp2 = 0;
  permutation = ipermutation = pers[n_nodes];
  stat = new int[2];
  stat[0] = stat[1] = 0;

  AutoGraph::ANode *e0 = new AutoGraph::ANode();

  // Empty graph
  e0->label = "";
  for (int i = 0; i < n_nodes * n_nodes; i++)
    e0->label += "0";

  e0->nei = new AEdge[nei_size];
  for (int i = 0; i < nei_size; i++)
    e0->nei[i].dest = NULL;

  graphMap[e0->label] = e0;

  cur = e0;

  if (_prebuild)
    prebuild();
}

AutoGraph::~AutoGraph()
{
  for (int i = 0; i < n_nodes; i++)
    delete[] adjM[i];
  delete[] adjM;
  delete[] tmpNauty;
  delete[] tmp2Nauty;
  delete[] stmp;
  delete[] stmp2;
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

void AutoGraph::prebuild()
{
  for (int i = 0; i < n_nodes; i++)
    for (int j = 0; j < n_nodes; j++)
      if (i != j && cur->nei[indexPair(i, j)].dest == NULL)
      {
        adjM[i][j] = 1;
        if (!directed)
          adjM[j][i] = 1;

        prebuild(cur, i, j);

        adjM[i][j] = 0;
        if (!directed)
          adjM[j][i] = 0;
      }
}

void AutoGraph::prebuild(AutoGraph::ANode* cur, int a, int b)
{
  AutoGraph::ANode* n;
  string s = runNauty();
  Perm ts;
  bool build = true;

  if (graphMap.count(s) != 0)
  {
    n = graphMap[s];
    ts = canonMap[s];
    build = false;
  }
  else
  {
    n = new AutoGraph::ANode();

    n->nei = new AEdge[nei_size];
    for (int i = 0; i < nei_size; i++)
      n->nei[i].dest = NULL;

    n->label = s;

    graphMap[s] = n;
    ts = tmp;
    canonMap[s] = ts;
  }

  Perm nperm = 0;
  Perm npermi = 0;

  nperm = invert(tmp);
  nperm = compose(nperm, ts);
  npermi = invert(nperm);

  int ct = 0;
  for (int i = 0; i < n_nodes; i++)
    for (int j = 0; j < i; j++)
      if (getPerm(nperm, i) < getPerm(nperm, j))
        ct++;
  stat[0] += ct <= 1;
  ct = 0;
  for (int i = 0; i < n_nodes; i++)
    for (int j = 0; j < i; j++)
      if (getPerm(npermi, i) < getPerm(npermi, j))
        ct++;
  stat[0] += ct <= 1;
  stat[1] += 2;

  int ai = getPerm(nperm, a), bi = getPerm(nperm, b);

  cur->nei[indexPair(a, b)] = {n, compress(nperm)};
  n->nei[indexPair(ai, bi)] = {cur, compress(npermi)};

  if (!build)
    return;

  Perm t = permutation;
  Perm it = ipermutation;
  compose(compress(nperm));

  list<pair<int, int> > ls, l2;
  for (int i = 0; i < n_nodes; i++)
    for (int j = 0; j < n_nodes; j++)
      if (i != j &&
          n->nei[indexPair(getPerm(permutation, i), getPerm(permutation, j))].dest == NULL)
      {
        if (!adjM[i][j])
          l2.push_back(make_pair(i, j));
        else
          ls.push_back(make_pair(i, j));
      }

  reverse(ls.begin(), ls.end());
  for (auto pi : l2)
    ls.push_back(pi);

  for (pair<int, int> pi : ls)
  {
    int i = pi.first;
    int j = pi.second;
    int ii = getPerm(permutation, i);
    int jj = getPerm(permutation, j);

    if (n->nei[indexPair(ii, jj)].dest == NULL)
    {
      adjM[i][j] = 1 - adjM[i][j];
      if (!directed)
        adjM[j][i] = 1 - adjM[j][i];

      prebuild(n, ii, jj);

      adjM[i][j] = 1 - adjM[i][j];
      if (!directed)
        adjM[j][i] = 1 - adjM[j][i];
    }
  }

  permutation = t;
  ipermutation = it;
}

string AutoGraph::runNauty()
{
  for (int i = 0; i < n_nodes; i++)
    for (int j = 0; j < n_nodes; j++)
      stmp[getPerm(permutation, i) * n_nodes + getPerm(permutation, j)] = adjM[i][j] ? '1' : '0';
  stmp[n_nodes * n_nodes] = '\0';

  iso->canonicalStrNauty(stmp, stmp2, tmpNauty, tmp2Nauty);
  string sv = stmp2;

  tmp = tmp2 = 0;
  for (int i = 0; i < n_nodes; i++)
  {
    setPerm(tmp, i, tmpNauty[i]);
    setPerm(tmp2, i, tmp2Nauty[i]);
  }

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
    for (int i = 0; i < nei_size; i++)
      n->nei[i].dest = NULL;

    n->label = s;

    graphMap[s] = n;;
  }

  Perm nperm = invert(tmp);
  Perm npermi = invert(nperm);

  for (int i = 0; i < n_nodes; i++)
    setPerm(tmp, getPerm(nperm, i), getPerm(tmp2, i));

  int ai = getPerm(nperm, a), bi = getPerm(nperm, b);

  int ct = 0;
  for (int i = 0; i < n_nodes; i++)
    for (int j = 0; j < i; j++)
      if (getPerm(nperm, i) < getPerm(nperm, j))
        ct++;
  stat[0] += ct <= 1;
  ct = 0;
  for (int i = 0; i < n_nodes; i++)
    for (int j = 0; j < i; j++)
      if (getPerm(npermi, i) < getPerm(npermi, j))
        ct++;
  stat[0] += ct <= 1;
  stat[1] += 2;

  cur->nei[indexPair(a, b)] = {n, compress(nperm)};
  n->nei[indexPair(ai, bi)] = {cur, compress(npermi)};

  compose(compress(nperm));

  if (n_nodes <= 10)
    return;

  string t = cur->label;
  t[a * n_nodes + b] = t[a * n_nodes + b] == '1' ? '0' : '1';
  t[b * n_nodes + a] = t[b * n_nodes + a] == '1' ? '0' : '1';

  for (int i = 0; i < n_nodes; i++)
    if (getPerm(tmp, i) == getPerm(tmp, ai))
      for (int j = (getPerm(tmp, ai) == getPerm(tmp, bi) ? i + 1 : 0); j < n_nodes; j++)
        if (getPerm(tmp, j) == getPerm(tmp, bi) && i != j)
        {
          if (n->nei[indexPair(i, j)].dest != NULL)
            continue;

          tmp2 = applyTranspositions(pers[n_nodes], ai, bi, i, j);

          for (int i = 0; i < n_nodes; i++)
            setPerm(tmp, i, getPerm(npermi, getPerm(tmp2, i)));

          int fl = 1;
          for (int x = 0; fl && x < n_nodes; x++)
            for (int y = 0; y < n_nodes; y++)
              if (s[x * n_nodes + y] != t[getPerm(tmp, x) * n_nodes + getPerm(tmp, y)])
              {
                fl = 0;
                break;
              }

          if (fl)
          {
            Perm npermorb = tmp;
            Perm npermorbi = 0;
            for (int i = 0; i < n_nodes; i++)
              setPerm(npermorbi, getPerm(npermorb, i), i);

            int ii = getPerm(npermorb, i), jj = getPerm(npermorb, j);
            cur->nei[indexPair(ii, jj)] = {n, compress(npermorbi)};
            n->nei[indexPair(i, j)] = {cur, compress(npermorb)};
          }
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
  a = getPerm(permutation, a);
  b = getPerm(permutation, b);

  AEdge e = cur->nei[indexPair(a, b)];

  if (e.dest == NULL)
  {
    createNeighbor(cur, a, b);
    e = cur->nei[indexPair(a, b)];
  }
  else
    compose(e.p);
  cur = e.dest;
}

Perm AutoGraph::applyTranspositions(Perm p, int a1, int a2, int b1, int b2)
{
  if (a1 > a2)
    swap(a1, a2);

  if (b1 > b2)
    swap(b1, b2);

  if (a2 == b1)
  {
    int t = getPerm(p, a1);
    setPerm(p, a1, getPerm(p, b2));
    setPerm(p, b2, t);
  }
  else if (a1 == b2)
  {
    int t = getPerm(p, a2);
    setPerm(p, a2, getPerm(p, b1));
    setPerm(p, b1, t);
  }
  else
  {
    int t = getPerm(p, a1);
    setPerm(p, a1, getPerm(p, b1));
    setPerm(p, b1, t);

    t = getPerm(p, a2);
    setPerm(p, a2, getPerm(p, b2));
    setPerm(p, b2, t);
  }

  return p;
}

Perm AutoGraph::compress(Perm perm)
{
  Perm res = 0;
  int fl = 1;
  int fc = -1;
  for (int i = 0; i < n_nodes; i++)
    if (i != fc && getPerm(perm, i) != i)
    {
      if (fl == 2)
      {
        fl = 0;
        break;
      }

      int a = i;
      int b = getPerm(perm, i);

      if (getPerm(perm, b) != a)
      {
        fl = 0;
        break;
      }

      res = a + (b << 4);

      fc = b;
      fl++;
    }

  return fl ? (res << 2) + fl : (perm << 2);
}

void AutoGraph::compose(Perm perm)
{
  if ((perm & 3) == 0)
  {
    perm >>= 2;
    Perm t = permutation;
    for (int i = 0; i < n_nodes; i++)
      setPerm(permutation, i, getPerm(perm, getPerm(t, i)));
    ipermutation = invert(permutation);
  }
  else if ((perm & 3) == 2)
  {
    perm >>= 2;
    int a = perm & 15;
    int b = perm >> 4;
    int ai = getPerm(ipermutation, a);
    int bi = getPerm(ipermutation, b);

    setPerm(permutation, bi, a);
    setPerm(permutation, ai, b);

    setPerm(ipermutation, a, bi);
    setPerm(ipermutation, b, ai);
  }
}

Perm AutoGraph::compose(Perm p1, Perm p2)
{
  Perm t = 0;
  for (int i = 0; i < n_nodes; i++)
    setPerm(t, i, getPerm(p2, getPerm(p1, i)));
  return t;
}

Perm AutoGraph::invert(Perm p)
{
  Perm t = 0;
  for (int i = 0; i < n_nodes; i++)
    setPerm(t, getPerm(p, i), i);
  return t;
}

int AutoGraph::find(Perm p, int a)
{
  for (int i = 0; i < n_nodes; i++)
    if (getPerm(p, i) == a)
      return i;
  return -1;
}

Perm AutoGraph::minimize(Perm p, Perm orbits)
{
  for (int i = 0; i < n_nodes; i++)
  {
    int b = find(p, i);
    if (getPerm(p, i) != i && getPerm(orbits, i) == getPerm(orbits, b))
    {
      setPerm(p, b, getPerm(p, i));
      setPerm(p, i, i);
    }
  }

  return p;
}

inline int AutoGraph::indexPair(int a, int b)
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
