#!/usr/bin/env python

import numpy as np
import matplotlib.pyplot as plt
import subprocess, random, string

'''exps = [
  ("ER", False, 6),
  ("ER", False, 7),
  ("ER", False, 8),
  ("PR", False, 6),
  ("PR", False, 7),
  ("PR", False, 8),
  ("SP", False, 5),
  ("SP", False, 6),
  ("SP", False, 7),
  ("ER", True, 4),
  ("ER", True, 5),
  ("PR", True, 4),
  ("PR", True, 5)
]'''


def run_command(rfile, cmd):
  print cmd
  f = open(rfile, "wb")
  subprocess.call(cmd.split(), stdout=f)
  f.close()

def take_time(is_main, rfile):
  if is_main:
    cmd = "./ISO_m"
  else:
    cmd = "./ISO_b"

  f = open(rfile, "r")
  fo = open("out" + rfile, "wb")
  subprocess.call(cmd, stdin=f, stdout=fo)
  f.close()
  fo.close()

  fo = open("out" + rfile, "r")
  ot = float(fo.read())
  fo.close()

  return ot

exps = [("PR", False, 6)]

gens = {
  'ER' : './er_gen.py %d %d %d',
  'PR' : './pr_gen.py %d %d %d',
  'SW' : './sw_gen.py %d %d %d'
}

lo = 1000
hi = 1000000

if __name__ == "__main__":
  for exp in exps:
    run_base = []
    run_meth = []
    times = []

    command = gens[exp[0]]
    direct = bool(exp[1])
    sz = exp[2]
    rfile = ''.join(random.SystemRandom().choice(string.ascii_uppercase + string.digits) for _ in range(6)) + '.txt'

    i = lo
    while i <= hi:
      print "Go for %d" % i
      run_command(rfile, command % (sz, direct, i))

      times.append(i)
      run_base.append(take_time(False, rfile))
      run_meth.append(take_time(True, rfile))

      i *= 5

    subprocess.call(["rm", rfile])
    subprocess.call(["rm", "out" + rfile])

    plt.figure(figsize=(8, 6), dpi=80)
    ax = plt.subplot(1, 1, 1)

    plt.plot(times, run_base, color="blue", linewidth=1.0, linestyle="-", marker='s')
    plt.plot(times, run_meth, color="green", linewidth=1.0, linestyle="-", marker='o')
    ax.set_xscale('log')

    # plt.savefig("exercice_2.png", dpi=72)
    plt.show()
