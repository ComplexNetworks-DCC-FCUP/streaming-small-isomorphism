# Experimental Analysis

To confirm the behaviour of the streaming canonisation method, the
code was ran against a baseline naive method with different graph
streams (cf. the original paper).

In this directory we include 4 stream generators:

 * `er_gen.py`, a simple Erdos-Renyi model based generator;
 * `pr_gen.py`, a generator based on a preferential attachment rule;
 * `sw_gen.py`, a generator that does edge swaps;
 * `ers_gen.py`, a generator based on the first model that varies the
   stream step and fixes the vertex number at 6;

The generators return an edge list, preceded by a line with 3
integers: the number of vertices, the direction of the network (0 -
undirected, 1 directed) and the step (as described in the original
paper).

## Analyser

The analyser file, `analyser.py`, assumes the existence of three
binaries, `ISO_b`, `ISO_m` and `ISO_p`, that run all versions of the
code (the naive and automaton method without and with pre-building,
respectively) and output the elapsed time. They must accept as input
the format returned by the generators.

To generate these, there is a `Makefile` in this directory that
creates the necessary binaries with the correct names, as used by the
analysis present in the paper.
