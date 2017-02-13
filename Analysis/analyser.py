#!/usr/bin/env python

######
#
# Experimental analysis
#
######

import matplotlib
matplotlib.use('Agg')

import numpy as np
import matplotlib.pyplot as plt
import subprocess, random, string

undirMain = [("ER", False, 6, "ER-6"),
             ("ER", False, 7, "ER-7"),
             ("ER", False, 8, "ER-8"),
             ("PR", False, 6, "PR-6"),
             ("PR", False, 7, "PR-7"),
             ("PR", False, 8, "PR-8")
]

dirMain = [("ER", True, 4, "dER-4"),
           ("ER", True, 5, "dER-5"),
           ("PR", True, 4, "dPR-4"),
           ("PR", True, 5, "dPR-5")
]

undirSW = [("SW", False, 5, "SW-5"),
           ("SW", False, 6, "SW-6"),
           ("SW", False, 7, "SW-7")
]

stepMain = [("ER2", False, 1, "ERs-1"),
            ("ER2", False, 2, "ERs-2"),
            ("ER2", False, 3, "ERs-3"),
            ("ER2", False, 4, "ERs-4"),
            ("ER2", False, 5, "ERs-5"),
            ("ER2", False, 6, "ERs-6")
]

##############
#
# Control Panel
#
##############

exps = stepMain
fname = "stepMain"
mult = 3
lo = 10000
hi = 10000000

##############

def run_command(rfile, cmd):
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

params = {'axes.labelsize': 24,
          'font.size': 20,
          'legend.fontsize': 20,
          'xtick.labelsize': 16,
          'ytick.labelsize': 16,
          'text.usetex': True}
plt.rcParams.update(params)

palette = [("#1abc9c", "#16a085"),
           ("#2ecc71", "#27ae60"),
           ("#3498db", "#2980b9"),
           ("#9b59b6", "#8e44ad"),
           ("#34495e", "#2c3e50"),
           ("#e67e22", "#d35400"),
           ("#e74c3c", "#c0392b")
]

gens = {
  'ER'  : './er_gen.py %d %d %d',
  'PR'  : './pr_gen.py %d %d %d',
  'SW'  : './sw_gen.py %d %d %d',
  'ER2' : './ers_gen.py %d %d %d'
}

if __name__ == "__main__":
  fig = plt.figure(figsize=(16, 9), dpi=120)
  ax = plt.subplot(1, 1, 1)
  ordn = 1

  base_list = []
  meth_list = []
  times_list = []

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
      tim = i + (random.randint(0, i * 0.1) - i * 0.05)
      print "Go for %d" % tim
      run_command(rfile, command % (sz, direct, tim))

      times.append(tim)
      run_base.append(take_time(False, rfile))
      run_meth.append(take_time(True, rfile))

      i *= mult

    subprocess.call(["rm", rfile])
    subprocess.call(["rm", "out" + rfile])

    colB, colO = palette[ordn % len(palette)]
    ordn += 1
    lab = exp[3]
    plt.plot(times, run_base, color=colB, mec=colB, mfc='none', mew=2.0, marker='s', ls='--', label='{\\tt B-'+lab+'}')
    plt.plot(times, run_meth, color=colO, mec=colO, mfc='none', mew=2.0, marker='o', label='{\\tt O-'+lab+'}')

    times_list.append(times)
    base_list.append(run_base)
    meth_list.append(run_meth)
    
  ax.set_xscale('log')
  ax.legend(loc='center left')
  ax.set_xlabel("Stream size ($|S|$)")
  ax.set_ylabel("Runtime (seconds)")
  ax.set_xlim([lo - lo * 0.2, hi + hi * 0.1])

  plt.savefig(fname + ".eps", dpi=120)
#  plt.show()

  f = open(fname + "_backup.txt", "wb")
  for i in times_list:
    for j in i:
      print >>f, j,
    print >>f, ""
  for i in base_list:
    for j in i:
      print >>f, j,
    print >>f, ""
  for i in meth_list:
    for j in i:
      print >>f, j,
    print >>f, ""
  f.close()
