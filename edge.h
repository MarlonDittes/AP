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

        int getDestination(int s){
            if (s == source){
                return destination;
            } else {
                return source;
            }
        }

        bool getArcFlag(int s, int k){
            if (s == source){
                return arcFlagsForwards[k];
            } else {
                return arcFlagsBackwards[k];
            }
        }

        void setBackwardArcFlag(int d, int k){
            if (d == source){
                arcFlagsForwards[k] = 1;
            } else {
                arcFlagsBackwards[k] = 1;
            }
        }
};

#endif
