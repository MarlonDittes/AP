#ifndef GRAPH_H
#define GRAPH_H
#include <iostream>
#include <vector>

class Graph{
    private:
        std::pair<std::vector<int>, std::vector<int>> adjacencyArray;
    public:
        Graph(const std::vector<std::pair<int, int>>& EdgeList, int n, int m);

        std::pair<std::vector<int>, std::vector<int>> getAdjacencyArray() { return this->adjacencyArray; };

        void printGraph();
        void printGraphShort(int length);
};

#endif
