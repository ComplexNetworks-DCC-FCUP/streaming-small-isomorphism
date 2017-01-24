#!/usr/bin/env python

import random
import sys
import numpy as np

def normalize(v):
  sm = sum(v)
  return [i / (1.0 * sm) for i in v]

def deg(v, edges):
  ct = 0
  for i in edges:
    if i[0] == v or i[1] == v:
      ct += 1
  return ct

if len(sys.argv) < 4:
  exit(0)

n = int(sys.argv[1])
d = int(sys.argv[2])
m = int(sys.argv[3])

print n, d, 1

edges = []
nedges = []
mx = 2 * (n - 1)
if d == 0:
  mx = n - 1
pdeg = []
for i in range(n):
  pdeg.append(0)

for i in range(n):
  for j in range(i + 1, n):
    nedges.append((i, j))
    if d == 1:
      nedges.append((j, i))

for i in range(m):
  tp = random.randint(0, 1)

  if len(edges) == 0:
    tp = 1
  elif len(nedges) == 0:
    tp = 0

  if tp == 0:
    # remove
    probs = []
    for e in edges:
      p = 1 + 2 * mx - pdeg[e[0]] + pdeg[e[1]]
      probs.append(p)

    probs = normalize(probs)
    sl = np.random.choice(range(len(edges)), 1, p = probs)[0]
    sl = edges[sl]

    pdeg[sl[0]] -= 1
    pdeg[sl[1]] -= 1
    edges.remove(sl)
    nedges.append(sl)

    print sl[0] + 1, sl[1] + 1
  else:
    # add
    probs = []
    for e in nedges:
      p = 1 + pdeg[e[0]] + pdeg[e[1]]
      probs.append(p)

    probs = normalize(probs)
    sl = np.random.choice(range(len(nedges)), 1, p = probs)[0]
    sl = nedges[sl]

    pdeg[sl[0]] += 1
    pdeg[sl[1]] += 1
    nedges.remove(sl)
    edges.append(sl)

    print sl[0] + 1, sl[1] + 1
