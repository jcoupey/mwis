# Maximum weight independent set (MWIS)

A MWIS of a set of vertices in a graph is a subset in wich no two
vertices are adjacent (that's an independent set) and such that the
sum of the vertices weight is maximal among all independent sets.

## MWIS in a path

Let _p_ be a path in an undirected graph (in wich no vertex appears
twice). We obtain the maximum weight for an independent set of _p_
using dynamic programming by computing the maximum weight for
sub-paths of _p_. Sub-paths are obtained adding vertices one at a
time, in their order of appearance in _p_.

Once the maximum weights are known for all such sub-paths, the list of
vertices composing the MWIS for _p_ can be retrieved by browsing the
list of maximum weights backwards.

Thus computing the maximum weight for an independent set of _p_ and
retrieving the corresponding MWIS is **performed with linear
complexity**.

## MWIS in a graph

### Background

When not restricting to a path (see above), the problem of finding a
maximum weight independent set in a graph is NP-hard, so approximation
algorithms are provided here.

### Weight of a mwis

For a vertex _v_ in the graph, let's denote by _W(v)_ and _d(v)_ the
weight and the degree of _v_ in the graph. Then _alpha_, the sum over
the vertices _v_ of _W(v)/(d(v) + 1)_ is a minorant for the weight of
a maximum weight independent set.

### Algorithms

The two greedy algorithms implemented are based on algorithms GWMIN
and GWMAX described in [this
article](http://www.sciencedirect.com/science/article/pii/S0166218X02002056). They
both return in any case an independent set whose weight is greater
than _alpha_.

## Usage

### Build

To build the executable, run in the source folder:

```bash
mkdir bin && cd src/ && make && cd ..
```

### Examples of mwis in a path

Toy around with a few examples using:

```bash
./bin/mwis -p
```

See ```main``` file to modify graph and paths used in the examples.

### Examples of approximations algorithms in a graph

Use:

```bash
./bin/mwis -g size
```

to build a random graph with ```size``` vertices and log the result of
the two algorithms on this graph.