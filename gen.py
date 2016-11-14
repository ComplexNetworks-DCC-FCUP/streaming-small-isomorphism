import random
import sys

if len(sys.argv) < 2:
  exit(0)

m = int(sys.argv[1])

print 3

for i in range(m):
  a = random.randint(1, 3)
  b = random.randint(1, 3)

  while a == b:
    b = random.randint(1, 3)

  print a, b
