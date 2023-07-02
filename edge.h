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
        double distance;
        std::vector<bool> arcFlags;

        int other(int node){
            if (node == source){
                return destination;
            } else if (node == destination){
                return source;
            } else {
                std::cout << "This node is not part of this Edge." << std::endl;
                return 0;
            }
        }

};

#endif
