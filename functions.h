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
void printAdjArr (std::pair<std::vector<int>, std::vector<int>> adjacencyArray);

// Reading Graph and Coordinate File
std::tuple<std::vector<Edge>, int, int> readGraphFile (std::string filename);
std::vector<Node> readCoordFile (std::string filename);

// Euklidische Norm, Visited = false für alle Nodes, Dijkstra und printParentPath
double eukld(Node u, Node v);
void computeDistances(std::vector<Edge>& EdgeList, std::vector<Node>& nodeArray);
void allVisitedToFalse (std::vector<Node>& nodeArray);
std::pair<std::vector<double>, std::vector<int>> Dijkstra (int source, int target, std::pair<std::vector<int>, std::vector<Edge*>>& graph, std::vector<Node>& nodeArray);
std::pair<std::vector<double>, std::vector<Edge*>> ArcFlagsDijkstra (int source, int target, std::pair<std::vector<int>, std::vector<Edge*>>& graph, std::vector<Node>& nodeArray);

// ArcFlags Funktionen
void readPartitionFile(std::string filename, std::vector<Node>& nodeArray);
std::vector<Edge*> modifiedDijkstra (int source, std::pair<std::vector<int>, std::vector<Edge*>>& graph, std::vector<Node>& nodeArray);
void initEdgeArcflags (std::vector<Edge>& EdgeList, int k);
void saveArcFlags(std::vector<Edge>& EdgeList, int k, std::string filename);
void readArcFlags(std::vector<Edge>& EdgeList, std::string filename);
void computeArcFlags(std::vector<Edge>& EdgeList, std::pair<std::vector<int>, std::vector<Edge*>>& graph, std::vector<Node>& nodeArray, int n);
void parallelComputeArcFlags(std::vector<Edge>& EdgeList, std::pair<std::vector<int>, std::vector<Edge*>>& graph, std::vector<Node>& nodeArray, int n);
#endif
