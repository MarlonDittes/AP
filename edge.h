#ifndef EDGE_H
#define EDGE_H
#include <iostream>
#include <vector>

class Edge{
    public:
        Edge():
            source(-1), destination(-1){};

        Edge(int s, int d):
            source(s), destination(d){};


        int source;
        int destination;
        int backwardEIndex = -1;
        double distance;
        std::vector<bool> arcFlags;
};

#endif
