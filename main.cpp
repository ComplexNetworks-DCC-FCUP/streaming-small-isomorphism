#include <stdio.h>
#include <string>
#include "IsoGraph.h"
#include "NautyGraph.h"

using namespace std;

int main()
{
  int n;
  scanf("%d", &n);

  IsoGraph* g = new NautyGraph(false, n);

  int a, b;
  while (scanf("%d %d", &a, &b) != EOF)
  {
    a--, b--;
    g->toggle(a, b);
    printf("%s\n", g->canonicalLabel().c_str());
  }

  return 0;
}
