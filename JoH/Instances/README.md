# kMIS
Instances generate by Gilbert' model for "Reactive VNS algorithm for the maximum k-subset intersection problem".
http://dx.doi.org/10.1007/s10732-020-09452-y

Graph G=(L \cup R, E), with L a collection of subsets of a set R. L \cup R are vertices and E edges.

We generate 3 types of instances.

Type 1: |L| = |R|
Type 2: |L| > |R|
Type 3: |L| < |R|

The full description of how the instances were generated can be found in section 6 of the paper.

In each file, the first line will contain |L| |R| |E| k.
From the second line, we will have the edges of the graph (u,v) with u \in L and v \in R.

For an instance of the same class and size, but with a different k value, the bipartite graph can be different.

