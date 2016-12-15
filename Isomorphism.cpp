#include "Isomorphism.h"
#include <stdio.h>

DEFAULTOPTIONS(options);
statsblk(stats);
__thread graphnau mm[MAXN*MAXM];

void Isomorphism::initNauty(int size, bool directed)
{
  n = size;
  m = (n + WORDSIZE - 1) / WORDSIZE;
  nauty_check(WORDSIZE,m,n,NAUTYVERSIONID);
  
  dir = directed;

  options.getcanon = TRUE;
  options.writeautoms = FALSE;
  if (dir) options.digraph = TRUE;
  else     options.digraph = FALSE;
}

void Isomorphism::finishNauty()
{
  nauty_freedyn();
  naugraph_freedyn();
  nautil_freedyn();
}

void Isomorphism::canonicalStrNauty(char* s, char* res, int* perm, int* orbs)
{
  int i, j, aux;

  for (int i = 0; i < n; i++)
  {
    gv = GRAPHROW(g, i, m);
    EMPTYSET(gv, m);

    for (int j = 0; j < n; j++)
      if (s[i * n + j] == '1')
        ADDELEMENT(gv, j);
  }

  if (perm == NULL)
    perm = lab;

  if (orbs == NULL)
    orbs = orbits;

  nauty(g, perm, ptn, NULL, orbs, &options, &stats,
        workspace, WORKSPACE_SIZE, m, n, mm);
  
  aux = 0;
  for (i = 0; i < n; i++)
  {
    gv = GRAPHROW(mm, i, m);
    for (j = 0; j < n; j++)
      res[aux++] = ISELEMENT(gv, j) ? '1' : '0';
  }
  res[aux] = '\0';
}
