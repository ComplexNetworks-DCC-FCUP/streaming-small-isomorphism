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

undirMain = [("ER", False, 6, "ER6"),
             ("ER", False, 7, "ER7"),
             ("ER", False, 8, "ER8"),
             ("PR", False, 6, "PR6"),
             ("PR", False, 7, "PR7"),
             ("PR", False, 8, "PR8")
]

dirMain = [("ER", True, 4, "dER-4"),
           ("ER", True, 5, "dER-5"),
           ("PR", True, 4, "dPR-4"),
           ("PR", True, 5, "dPR-5")
]

undirSW = [("SW", False, 5, "SW5"),
           ("SW", False, 6, "SW6"),
           ("SW", False, 7, "SW7")
]

##############
#
# Control Panel
#
##############

exps = undirMain
fname = "undirMain"
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

params = {'axes.labelsize': 14,
          'font.size': 14,
          'legend.fontsize': 14,
          'xtick.labelsize': 12,
          'ytick.labelsize': 12,
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
  'ER' : './er_gen.py %d %d %d',
  'PR' : './pr_gen.py %d %d %d',
  'SW' : './sw_gen.py %d %d %d'
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
      print j,
    print ""
  for i in base_list:
    for j in i:
      print j,
    print ""
  for i in meth_list:
    for j in i:
      print j,
    print ""
  f.close()
