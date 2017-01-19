#!/usr/bin/env python

import random
import sys

if len(sys.argv) < 4:
  exit(0)

n = int(sys.argv[1])
d = int(sys.argv[2])
m = int(sys.argv[3])

print n, d, 1

for i in range(m):
  a = random.randint(1, n)
  b = random.randint(1, n)

  while a == b:
    b = random.randint(1, n)

  print a, b
