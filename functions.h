#ifndef GRAPH_H
#define GRAPH_H
#include <iostream>
#include <vector>
#include "node.h"
#include "edge.h"

// Creating and printing Adjacency Array
// First of Pair is the Graph Data Strucutre, so the adjacency array
// Second of Pair is the EdgeIndices Array, used in ArcFlags functionality
std::pair<std::vector<int>, std::vector<int>>  createAdjArr(const std::vector<Edge>& EdgeList, int n, int m);
void printAdjArr (std::pair<std::vector<int>, std::vector<int>> adjacencyArray);

// Reading Graph and Coordinate File
std::tuple<std::vector<Edge>, int, int> readGraphFile (std::string filename);
std::vector<Node> readCoordFile (std::string filename);

// Euklidische Norm, Visited = false für alle Nodes, Dijkstra und printParentPath
double eukld(Node u, Node v);
void allVisitedToFalse (std::vector<Node>& nodeArray);
std::pair<std::vector<double>, std::vector<int>> Dijkstra (int source, int target, const std::pair<std::vector<int>, std::vector<int>>& graph, std::vector<Node>& nodeArray);
std::pair<std::vector<double>, std::vector<int>> ArcFlagsDijkstra (int source, int target, const std::pair<std::vector<int>, std::vector<int>>& graph, std::vector<Node>& nodeArray, std::vector<std::vector<std::pair<int, int>>> edgeIndices, std::vector<bool> arcFlags, int k);
void printParentPath (const std::pair<std::vector<double>, std::vector<int>>& distanceAndParent, int source, int target);

// ArcFlags Funktionen
void readPartitionFile(std::string filename, std::vector<Node>& nodeArray);
std::vector<int> modifiedDijkstra (int source, const std::pair<std::vector<int>, std::vector<int>>& graph, std::vector<Node>& nodeArray);
void saveArcFlags(std::vector<bool> arcFlags, int m, int k, std::string filename);
std::vector<bool> readArcFlags(std::string filename, int m, int k);
std::vector<bool> computeArcFlags(const std::pair<std::vector<int>, std::vector<int>>& graph, std::vector<Node>& nodeArray, std::vector<std::vector<std::pair<int, int>>> edgeIndices, int m, int k);

#endif
