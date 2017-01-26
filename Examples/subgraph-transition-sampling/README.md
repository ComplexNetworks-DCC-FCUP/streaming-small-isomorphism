# Case Study: Sampling of subgraph transitions

This example is meant to serve as a use case of the streaming small
graph canonisation method. It is described in detail in Appendix B of
the original paper.

## Brief description

A sample of subgraph transitions in a temporal network is
performed. For a given graph, this means calculating an estimation of
a Markov chain of graphs that represent temporal transitions of the
isomorphism class of the subgraphs of the given graph. More
information is available on the original paper.

The networks included in the `Networks` directory are taken from
[SNAP](http://snap.stanford.edu/data/index.html) and
[Konect](http://konect.uni-koblenz.de/), for more information check
the original paper and its citations.

## Usage

Here we reproduce the help information provided by the implementation:

```
------------ TSUB Usage --------------
Main Settings: ./TSUB -s <Subgraph Size> [arguments...]

	All commands:
-h : Displays this help information
-s <Integer> : Subgraph Size
-d : Directed graph (Default undirected)
-m <Integer> : Base method (0 - naive [default], 1 - automaton)
-t <Integer> : Number of samples (default 1,000,000)
-f <Integer> : Fix the randomness seed (default is time based)
--------------------------------------
```

There options setup the tool that accepts files in the same format as
the networks in the `Networks` directory, which is `ID1 ID2 Time`,
where: `ID1` is the id of the first vertex, which must be between 1
and the number of vertices; `ID2` is the id of the second vertex,
which must be between 1 and the number of vertices (note that the
order matters in directed networks); `Time` is a timestamp that
represents when that edge was created.

An example run: `./TSUB -s 5 -m 1 -f 22 < Networks/infectious.edges`

## Output

The output format of the code is something like:

```
Transition List:
	0000000000000000 -> 0000000000010010: 1000000
	0000000000010010 -> 0000000100010110: 1000000
	0000000100010110 -> 0000001101010110: 273401
	0000000100010110 -> 0001000100011110: 355861
	0000000100010110 -> 0010000110010110: 370738
	0000001101010110 -> 0001001101011110: 273401
	0001000100011110 -> 0001001101011110: 248324
	0001001101011110 -> 0011001111011110: 332586
	0010000110010110 -> 0001001101011110: 131632
	0010000110010110 -> 0110100110010110: 30159
	0011001111011110 -> 0111101111011110: 162364
	0110100110010110 -> 0011001111011110: 8069

```

Where each transition has a source and a target canonical label (as
given by the canonisation method) and the integer associated is the
number of transitions of this kind found by the sampling.
