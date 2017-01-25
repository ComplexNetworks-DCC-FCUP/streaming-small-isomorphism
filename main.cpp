#include <stdio.h>
#include <string>
#include <assert.h>
#include "IsoGraph.h"
#include "NautyGraph.h"
#include "AutoGraph.h"
#include "Timer.h"

using namespace std;

int main()
{
  int n, d, tp, pre;
  scanf("%d %d %d %d", &n, &d, &tp, &pre);

  IsoGraph* g;

  Timer::start();
  if (tp == 0)
    g = new NautyGraph(d, n);
  else
    g = new AutoGraph(d, n, pre);

  int a, b;
  while (scanf("%d %d", &a, &b) != EOF)
  {
    a--, b--;

    g->toggle(a, b);
    string s = g->canonicalLabel().c_str();

    printf("Canonical Type: %s\n", s.c_str());
  }

  Timer::stop();

  printf("Runtime: %0.4lf\n", Timer::elapsed());

  return 0;
}
