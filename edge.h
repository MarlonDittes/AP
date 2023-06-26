#ifndef EDGE_H
#define EDGE_H
#include <iostream>
#include <vector>

class Edge{
    public:
        Edge():
            source(-1), destination(-1), index(-1){};

        Edge(int s, int d, int i):
            source(s), destination(d), index(i){};


        int source;
        int destination;
        int index;
        int backwardsEdgeIndex = -1;

};

#endif
