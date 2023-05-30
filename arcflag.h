#ifndef ARCFLAG_H
#define ARCFLAG_H
#include <iostream>
#include <vector>
#include "functions.h"

class ArcFlag{
    private:
        int partitionSize;


    public:
        std::vector<int> mapping;
        std::vector<bool> arcFlagBits;

        int getPartitionSize(){ return this->partitionSize;};
};

#endif
