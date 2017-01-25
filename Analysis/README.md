# Experimental Analysis

To confirm the behaviour of the streaming canonisation method, the
code was ran against a baseline naive method with different graph
streams (cf. the original paper).

In this directory we include 3 stream generators:

 * `er_gen.py`, a simple Erdos-Renyi model based generator;
 * `pr_gen.py`, a generator based on a preferential attachment rule;
 * `sw_gen.py`, a generator that does edge swaps;

The generators return an edge list, preceded by a line with 3
integers: the number of vertices, the direction of the network (0 -
undirected, 1 directed) and the step (as described in the original
paper).

## Analyser

The analyser file, `analyser.py`, assumes the existence of two
binaries, `ISO_b`and `ISO_m`, that run both versions of the code (the
naive and automaton method, respectively) and output the elapsed
time. They must accept as input the format returned by the generators.
