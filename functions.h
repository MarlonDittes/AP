#ifndef GRAPH_H
#define GRAPH_H
#include <iostream>
#include <vector>
#include "node.h"
#include "edge.h"

// Creating and printing Adjacency Array
// First of Pair is the Graph Data Strucutre, so the adjacency array
// Second of Pair is the EdgeIndices Array, used in ArcFlags functionality
std::pair<std::vector<int>, std::vector<Edge*>>  createAdjArr(std::vector<Edge>& EdgeList, int n, int m);

// Reading Graph and Coordinate File
std::tuple<std::vector<Edge>, int, int> readGraphFile (std::string filename);
std::vector<Node> readCoordFile (std::string filename);

// Euklidische Norm, Visited = false für alle Nodes, Dijkstra und printParentPath
double eukld(Node u, Node v);
void computeDistances(std::pair<std::vector<int>, std::vector<Edge*>>& graph, std::vector<Node>& nodeArray);
void allVisitedToFalse (std::vector<Node>& nodeArray);
std::pair<std::vector<double>, std::vector<Edge*>> Dijkstra (int source, int target, std::pair<std::vector<int>, std::vector<Edge*>>& graph, std::vector<Node>& nodeArray);
std::pair<std::vector<double>, std::vector<Edge*>> AStarDijkstra (int source, int target, std::pair<std::vector<int>, std::vector<Edge*>>& graph, std::vector<Node>& nodeArray);
std::pair<std::vector<double>, std::vector<Edge*>> ArcFlagsDijkstra (int source, int target, std::pair<std::vector<int>, std::vector<Edge*>>& graph, std::vector<Node>& nodeArray);
std::pair<std::vector<double>, std::vector<Edge*>> AStarArcFlagsDijkstra (int source, int target, std::pair<std::vector<int>, std::vector<Edge*>>& graph, std::vector<Node>& nodeArray);

// ArcFlags Funktionen
void readPartitionFile(std::string filename, std::vector<Node>& nodeArray);
std::vector<Edge*> modifiedDijkstra (int source, std::pair<std::vector<int>, std::vector<Edge*>>& graph, std::vector<Node>& nodeArray);
void initEdgeArcflags (std::pair<std::vector<int>, std::vector<Edge*>>& graph, int k);
void saveArcFlags(std::pair<std::vector<int>, std::vector<Edge*>>& graph, int k, std::string filename);
void readArcFlags(std::pair<std::vector<int>, std::vector<Edge*>>& graph, std::string filename);
void computeArcFlags(std::vector<Edge>& EdgeList, std::pair<std::vector<int>, std::vector<Edge*>>& graph, std::vector<Node>& nodeArray, int n);
void parallelComputeArcFlags(std::vector<Edge>& EdgeList, std::pair<std::vector<int>, std::vector<Edge*>>& graph, std::vector<Node>& nodeArray, int n);

// Testing
std::vector<std::pair<int, int>> readTestFile (std::string filename);
void saveResults(std::vector<std::pair<double, long long>> distanceAndTime, std::string filename, int k, std::string type);
void saveExploredNodes(std::vector<int> exploredNodes, std::string filename, std::vector<Node>& nodeArray);
#endif
