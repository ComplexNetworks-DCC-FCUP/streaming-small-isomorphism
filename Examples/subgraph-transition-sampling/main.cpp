#include <stdio.h>
#include <string>
#include <vector>
#include "../../IsoGraph.h"
#include "../../NautyGraph.h"
#include "../../AutoGraph.h"
#include "../../Timer.h"
#include "Graph.h"

using namespace std;

typedef pair<int, int> Edge;
typedef pair<Edge, int> TEdge;

vector<Edge> edgeList;
map<pair<string, string>, int> transitions;
int timestep = 1;

void displayHelp()
{
 printf("------------ TSUB Usage --------------\n"
        "Main Settings: ./TSUB -s <Subgraph Size> [arguments...]\n\n"
        "\tAll commands:\n"
        "-h : Displays this help information\n"
        "-s <Integer> : Subgraph Size\n"
        "-d : Directed graph (Default undirected)\n"
        "-m <Integer> : Base method (0 - naive [default], 1 - automaton)\n"
        "-t <Integer> : Number of samples (default 1,000,000)\n"
        "-f <Integer> : Fix the randomness seed (default is time based)\n"
        "--------------------------------------\n");
}

void readGraph(Graph* g, bool dir)
{
  int i, a, b, c, size, max;
  vector<int> va, vb;

  size = max = 0;
  while (scanf("%d %d %d", &a, &b, &c) == 3) {
    va.push_back(a);
    vb.push_back(b);

    if (a > max) max = a;
    if (b > max) max = b;
    size++;
  }

  g->createGraph(max, dir);

  for (i = 0; i < size; i++)
  {
    if (va[i] == vb[i])
      continue; // discard self loops!

    if (!g->hasEdge(va[i] - 1, vb[i] - 1))
    {
      g->addEdge(va[i] - 1, vb[i] - 1, c); // discard multiple edges!

      if (!dir)
        g->addEdge(vb[i] - 1, va[i] - 1, c);
    }
  }

  va.clear();
  vb.clear();

  g->prepareGraph();
}

void initSample(Graph* G)
{
  for (int i = 0; i < G->numNodes(); i++)
  {
    int* nei = G->arrayNeighbours(i);
    int nnei = G->arrayNumNeighbours(i);

    for (int j = 0; j < nnei; j++)
      if (nei[j] < i)
        edgeList.push_back(make_pair(i, nei[j]));
  }
}

// Samples using a Random Vertex Expansion
//
// Lu, Xuesong, and Stéphane Bressan.
// "Sampling connected induced subgraphs uniformly at random."
// International Conference on Scientific and Statistical Database Management.
// Springer Berlin Heidelberg, 2012.
//
vector<int> sampleSubgraph(Graph* G, int n)
{
  vector<int> subgraph;
  int ind = rand() % edgeList.size();
  subgraph.push_back(edgeList[ind].first);
  subgraph.push_back(edgeList[ind].second);

  vector<int> EL;
  while (subgraph.size() < n)
  {
    EL.clear();
    for (int i : subgraph)
    {
      int* nei = G->arrayNeighbours(i);
      int nnei = G->arrayNumNeighbours(i);

      for (int j = 0; j < nnei; j++)
        if (find(subgraph.begin(), subgraph.end(), nei[j]) == subgraph.end())
          EL.push_back(nei[j]);
    }

    if (EL.size() == 0)
      return sampleSubgraph(G, n);

    ind = rand() % EL.size();
    subgraph.push_back(EL[ind]);
  }

  return subgraph;
}

bool cmp(TEdge a, TEdge b)
{
  if (a.second == b.second)
    return a < b;
  return a.second < b.second;
}

void createTransitionSample(Graph* G, IsoGraph* iso, int n, bool d, int iterations)
{
  initSample(G);

  for (int i = 0; i < iterations; i++)
  {
    vector<int> subgraph = sampleSubgraph(G, n);
    iso->cleanup();

    vector<TEdge> streamList;
    int ind = 0;
    for (int i : subgraph)
    {
      int* nei = G->arrayNeighbours(i);
      int* tm = G->arrayTimNeighbours(i);
      int nnei = G->arrayNumNeighbours(i);

      for (int j = 0; j < nnei; j++)
        if (i < nei[j] && find(subgraph.begin(), subgraph.end(), nei[j]) != subgraph.end())
        {
          Edge p = make_pair(ind, int(find(subgraph.begin(), subgraph.end(), nei[j]) - subgraph.begin()));
          streamList.push_back(make_pair(p, tm[j]));
        }

      ind++;
    }

    sort(streamList.begin(), streamList.end(), cmp);

    string sp = iso->canonicalLabel();
    int delta = timestep;
    for (TEdge t : streamList)
    {
      string sa;

      iso->toggle(t.first.first, t.first.second);

      if (--delta == 0)
      {
        sa = iso->canonicalLabel();

        transitions[make_pair(sp, sa)]++;

        sp = sa;
        delta = timestep;
      }
    }
  }
}

int main(int argc, char **argv)
{
  int n = -1, m = 0, iterations = 1000000;
  int seed = time(NULL);
  bool d = false;

  for (int i = 1; i < argc; i++)
  {
    if (argv[i][0] != '-')
      continue;

    if (argv[i][1] == 'h')
    {
      displayHelp();
      return 0;
    }
    else if (argv[i][1] == 's')
    {
      i++;

      n = argv[i][0] - '0';
      int j = 1;
      while (argv[i][j] != '\0')
      {
	n *= 10;
	n += argv[i][j] - '0';
	j++;
      }
    }
    else if (argv[i][1] == 'd')
      d = true;
    else if (argv[i][1] == 'm')
    {
      i++;

      m = argv[i][0] - '0';
      int j = 1;
      while (argv[i][j] != '\0')
      {
	m *= 10;
	m += argv[i][j] - '0';
	j++;
      }
    }
    else if (argv[i][1] == 't')
    {
      i++;

      iterations = argv[i][0] - '0';
      int j = 1;
      while (argv[i][j] != '\0')
      {
	iterations *= 10;
	iterations += argv[i][j] - '0';
	j++;
      }
    }
    else if (argv[i][1] == 'f')
    {
      i++;

      seed = argv[i][0] - '0';
      int j = 1;
      while (argv[i][j] != '\0')
      {
	seed *= 10;
	seed += argv[i][j] - '0';
	j++;
      }
    }
    else
      printf("Option '%s' not recognized\n", argv[i]);
  }

  srand(seed);

  IsoGraph* iso;
  Graph* G = new Graph();

  Timer::start();

  readGraph(G, d);

  if (m == 0)
    iso = new NautyGraph(d, n);
  else
    iso = new AutoGraph(d, n);

  createTransitionSample(G, iso, n, d, iterations);

  Timer::stop();

  delete iso;
  delete G;

  printf("Transition List:\n");
  for (auto t : transitions)
    printf("\t%s -> %s: %d\n", t.first.first.c_str(), t.first.second.c_str(), t.second);
  printf("\n");

  printf("Runtime: %0.4lf\n", Timer::elapsed());

  return 0;
}
