# Streaming Small Graph Isomorphism

This software is an implementation of the methods described in the
following paper:

 * Fast streaming small graph canonisation, Pedro Paredes and Pedro Ribeiro (submitted)

It performs a streaming graph canonisation, meaning it canonises a set
of graphs in a stream of edge change operations (for more information
check the paper).

This software is meant to be used as an API, which will be detailed
bellow. We include an example application of this method in a graph
mining tool, which is available in the `Examples` directory.

In the `Analysis` directory we include the code used to perform the
experimental analysis reported in the original paper.

This software uses the nauty program version 2.4 by Brendan
McKay. Therefore, nauty's license restrictions apply to the usage of
this software.

## API

The automaton API is very simple and includes one virtual class and
two classes. The virtual class is called `IsoGraph` and has the
following functions:

 * `void toogle(int a, int b)`, that toggles an edge between `a` and
   `b`, from 1 to `n` (if the edge exists removes it, if it doesn't
   adds it);
   
 * `bool isConnected(int a, int b)`, that returns true if there is an
   edge between `a` and `b`, where both variable are from 1 to `n`;
   
 * `string canonicalLabel()`, returns a string that represents the
   canonical label of the graph currently represented by the object;
   
The available implementations of this virtual class are the two
following classes:

 * `NautyGraph`, with constructor `NautyGraph(bool direction, int n)`,
   where `direction` indicates if the graph is directed or not and `n`
   is the number of vertices. This is a simple implementation that
   runs _Nauty_ on each call to `canonicalLabel()`;
   
 * `AutoGraph`, with constructor `AutoGraph(bool direction, int n,
   bool prebuild)`, where `direction` indicates if the graph is
   directed or not, `n` is the number of vertices and `prebuild`
   indicated if the automaton should be prebuilt. This is the
   implementation of our method as described in the paper;

## Author Information

This software was created by researchers of the Faculty of Sciences of
the University or Porto, from the research group
[CRACS](http://cracs.fc.up.pt/).

At the time of publication, Pedro Paredes is a second year Computer
Science Master's student advised by professor Pedro Ribeiro.
