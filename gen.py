import random
import sys

if len(sys.argv) < 2:
  exit(0)

n = int(sys.argv[1])
m = int(random.random() * (n * (n + 1) / 2))

print n

for i in range(m):
  a = random.randint(1, n)
  b = random.randint(1, n)

  while a == b:
    b = random.randint(1, n)

  print a, b
