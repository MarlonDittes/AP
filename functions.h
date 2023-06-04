#ifndef GRAPH_H
#define GRAPH_H
#include <iostream>
#include <vector>
#include "node.h"
#include "edge.h"

// Creating and printing Adjacency Array
std::pair<std::pair<std::vector<int>, std::vector<int>>, std::vector<std::vector<std::pair<int, int>>>> createAdjArr(const std::vector<Edge>& EdgeList, int n, int m);
void printAdjArr (std::pair<std::vector<int>, std::vector<int>> adjacencyArray);

// Reading Graph and Coordinate File
std::pair<std::pair<std::vector<int>, std::vector<int>>, std::vector<std::vector<std::pair<int, int>>>> readGraphFile (std::string filename);
std::vector<Node> readCoordFile (std::string filename);

// Euklidische Norm, Visited = false für alle Nodes, Dijkstra und printParentPath
double eukld(Node u, Node v);
void allVisitedToFalse (std::vector<Node>& nodeArray);
std::pair<std::vector<double>, std::vector<int>> Dijkstra (int source, int target, const std::pair<std::vector<int>, std::vector<int>>& graph, std::vector<Node>& nodeArray);
void printParentPath (const std::pair<std::vector<double>, std::vector<int>>& distanceAndParent, int source, int target);

// ArcFlags Funktionen
std::vector<int> readPartitionFile(std::string filename);

#endif
