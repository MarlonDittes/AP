#ifndef GRAPH_H
#define GRAPH_H
#include <iostream>
#include <vector>
#include "node.h"

// Creating and printing Adjacency Array
std::pair<std::vector<int>, std::vector<int>> createAdjArr(const std::vector<std::pair<int, int>>& EdgeList, int n, int m);
void printAdjArr (std::pair<std::vector<int>, std::vector<int>> adjacencyArray);

// Reading Graph and Coordinate File
std::pair<std::vector<int>, std::vector<int>> readGraphFile (std::string filename);
std::vector<Node> readCoordFile (std::string filename);

#endif
