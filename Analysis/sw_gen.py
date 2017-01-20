#!/usr/bin/env python

import random
import sys
import numpy as np

def mn(p):
  return (min(p[0], p[1]), max(p[0], p[1]))

if len(sys.argv) < 4:
  exit(0)

n = int(sys.argv[1])
d = int(sys.argv[2])
m = int(sys.argv[3])

print n, d, 4

edges = []
nedges = []

for i in range(n):
  for j in range(i + 1, n):
    nedges.append((i, j))
    if d == 1:
      nedges.append((j, i))

sel = np.random.choice(range(len(nedges)), int(0.6 * len(nedges)), replace = False)
for i in sel:
  edges.append(nedges[i])
  print nedges[i][0] + 1, nedges[i][1] + 1

for i in range(m):
  sl1, sl2 = np.random.choice(range(len(edges)), 2, replace = False)
  sl1, sl2 = (edges[sl1], edges[sl2])

  edges.remove(sl1)
  edges.remove(sl2)

  nsl1 = (sl1[0], sl2[0])
  nsl2 = (sl1[1], sl2[1])

  while nsl1 in edges or nsl2 in edges or nsl1[0] == nsl1[1] or nsl2[0] == nsl2[1]:
    edges.append(sl1)
    edges.append(sl2)

    sl1, sl2 = np.random.choice(range(len(edges)), 2, replace = False)
    sl1, sl2 = (edges[sl1], edges[sl2])

    edges.remove(sl1)
    edges.remove(sl2)

    nsl1 = (sl1[0], sl2[0])
    nsl2 = (sl1[1], sl2[1])

  nsl1, nsl2 = mn(nsl1), mn(nsl2)
  edges.append(nsl1)
  edges.append(nsl2)

  print sl1[0] + 1, sl1[1] + 1
  print sl2[0] + 1, sl2[1] + 1
  print nsl1[0] + 1, nsl1[1] + 1
  print nsl2[0] + 1, nsl2[1] + 1
