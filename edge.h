#ifndef EDGE_H
#define EDGE_H
#include <iostream>
#include <vector>
#include <bitset>

#ifndef ARC_SIZE
#define ARC_SIZE 32 // Standardgröße des Bitsets, wenn ARC_SIZE nicht definiert ist
#endif

class Edge{
    public:
        Edge():
            source(-1), destination(-1){};

        Edge(int s, int d):
            source(s), destination(d){};


        int source;
        int destination;
        double distance;
        std::bitset<ARC_SIZE> arcFlagsForwards;
        std::bitset<ARC_SIZE> arcFlagsBackwards;

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

        void setBackwardArcFlag(int d, int k, bool b){
            if (d == source){
                arcFlagsForwards[k] = b;
            } else {
                arcFlagsBackwards[k] = b;
            }
        }
};

#endif
