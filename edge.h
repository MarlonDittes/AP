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
        std::vector<bool> arcFlagsForwards;
        std::vector<bool> arcFlagsBackwards;

        int getDestination(int n){
            if (n == source){
                return destination;
            } else {
                return source;
            }
        }

        bool getArcFlag(int n, int k){
            if (n == source){
                return arcFlagsForwards[k];
            } else {
                return arcFlagsBackwards[k];
            }
        }

        void setBackwardArcFlag(int n, int k){
            if (n == source){
                arcFlagsForwards[k] = 1;
            } else {
                arcFlagsBackwards[k] = 1;
            }
        }
};

#endif
