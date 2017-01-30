#include <stdio.h>
#include <string>
#include <assert.h>
#include "../IsoGraph.h"
#include "../NautyGraph.h"
#include "../AutoGraph.h"
#include "../Timer.h"

using namespace std;

int main()
{
  int n, d, step;
  scanf("%d %d %d", &n, &d, &step);

  IsoGraph* g;

  Timer::start();
  g = new NautyGraph(d, n);

  int a, b, ord = 0;
  while (scanf("%d %d", &a, &b) != EOF)
  {
    a--, b--;

    g->toggle(a, b);
    string s;
    if (ord++ % step == 0)
      s = g->canonicalLabel().c_str();
  }

  Timer::stop();

  printf("%0.4lf\n", Timer::elapsed());

  return 0;
}
