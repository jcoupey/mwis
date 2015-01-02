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


