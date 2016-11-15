#include <stdio.h>
#include <string>
#include <assert.h>
#include "IsoGraph.h"
#include "NautyGraph.h"
#include "AutoGraph.h"

using namespace std;

int main()
{
  int n;
  scanf("%d", &n);

  IsoGraph* g1 = new NautyGraph(false, n);
  IsoGraph* g2 = new AutoGraph(false, n);

  int a, b, ord = 1;
  while (scanf("%d %d", &a, &b) != EOF)
  {
    a--, b--;
    g1->toggle(a, b);
    g2->toggle(a, b);

    string s1 = g1->canonicalLabel().c_str();
    string s2 = g2->canonicalLabel().c_str();

//    printf("%s %s %d\n", s1.c_str(), s2.c_str(), ord++);
 
    assert(s1 == s2);
  }

  return 0;
}
