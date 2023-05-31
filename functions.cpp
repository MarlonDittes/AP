#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "node.h"
#include <cmath>
#include "heap.h"
#include <cassert>
#include <unordered_map>
#include "edge.h"

// Create Adjacency Array using an EdgeList
std::pair<std::vector<int>, std::vector<int>> createAdjArr(const std::vector<Edge>& EdgeList, int n, int m){
    // init V to be only 0's
    // init E with 2*m for case of undirected Graph
    std::vector<int> V(n+1, 0);
    std::vector<int> E(2*m);

    // count how many edges each node has and fill unordered Map
    int i=0;
    for (const auto& edge: EdgeList){
        V[edge.source]++;
    }

    // prefix sums for V
    for (int i = 1; i<n+1; i++){
        V[i]+=V[i-1];
    }

    // init E
    for (const auto& edge: EdgeList){
        E[--V[edge.source]] = edge.destination;
    }

    return std::make_pair(V, E);
}

// Functionality of printing Adjacency Array for testing
void printAdjArr (std::pair<std::vector<int>, std::vector<int>> adjacencyArray){
    std::cout << "Printing Graph: \n";

    // print V
    std::cout << "V: ";
    if (adjacencyArray.first.empty()){
        return;
    }
    std::cout << adjacencyArray.first[0];

    for (int i = 1; i<adjacencyArray.first.size(); i++){
        std::cout << ", " << adjacencyArray.first[i];
    }
    std::cout << std::endl;

    // print E
    std::cout << "E: ";
    if (adjacencyArray.second.empty()){
        return;
    }
    std::cout << adjacencyArray.second[0];

    for (int i = 1; i<adjacencyArray.second.size(); i++){
        std::cout << ", " << adjacencyArray.second[i];
    }
    std::cout << std::endl;
}

// Create AdjArr from Graph file
std::pair<std::vector<int>, std::vector<int>> readGraphFile (std::string filename){
    std::cout << "Reading Graph file..." << std::endl;
    std::string line;
    std::ifstream MyReadFile;
    MyReadFile.open(filename);
    std::vector<Edge> EdgeList;

    if (!MyReadFile){
        std::cout << "Couldn't read graph file " << filename << ", try providing full path." << std::endl;
        return createAdjArr(EdgeList, 0, 0);
    }

    // Read Number of Nodes (n) and Number of Edges (m)
    getline (MyReadFile, line);
    std::istringstream iss(line);
    int n, m;
    iss >> n;
    iss >> m;

    // Read edges, u counts at which line (Node) we are, v are corresponding neighbors
    // Since the graph files we read range from Indices 1 to n, but our Array V starts at 0
    // we have to decrease each Index by 1
    int u,v,i;
    u = 0;
    i = 0;

    while (getline (MyReadFile, line)){
        std::istringstream iss(line);
        while (iss >> v){
            v--;
            Edge e(u,v,i++);
            EdgeList.push_back(e);
        }
        u++;
    }
    auto graph = createAdjArr(EdgeList, n, m);
    return graph;
}

// Create NodeArray from coordinate file
std::vector<Node> readCoordFile (std::string filename){
    std::cout << "Reading Coordinate File..." << std::endl;
    std::string line;
    std::ifstream MyReadFile;
    MyReadFile.open(filename);
    std::vector<Node> NodeArray;

    if (!MyReadFile){
        std::cout << "Couldn't read coordinate file " << filename << ", try providing full path." << std::endl;
        return NodeArray;
    }

    // Read x and y coordinates for all nodes, start at Index i=0
    int i = 0;
    double x,y;
    while (getline (MyReadFile, line)){
        std::istringstream iss(line);
        // only read x,y; we don't care about z
        iss >> x;
        iss >> y;
        // create node with corresponding index, x ,y and push_back into nodeArray
        Node node(i, x, y);
        NodeArray.push_back(node);
        // increase index for next line
        i++;
    }
    // return nodeArray of Nodes
    return NodeArray;
}

// Distanz Berechnung mittels Euklidischer Norm
double eukld(Node* u, Node* v) {
    double norm = 0.0;
    norm += std::pow(u->getX() - v->getX(), 2.0);
    norm += std::pow(u->getY() - v->getY(), 2.0);
    return std::sqrt(norm);
}

// Set all visited of nodeArray to false
void allVisitedToFalse (std::vector<Node>& nodeArray){
    for (auto node : nodeArray){
        node.setVisited(false);
    }
}

// Dijkstra
std::pair<std::vector<double>, std::vector<int>> Dijkstra (int source, int target, const std::pair<std::vector<int>, std::vector<int>>& graph, std::vector<Node>& nodeArray){
    // Source, Target node index -1 so it fits into the data structure
    std::cout << "Performing Djikstra..." << std::endl;
    assert(source > 0 and target > 0);
    source--;
    target--;
    // Create distance, parent Array and aPQ
    // default distance is Infinity
    // default parent is bottom (here -1)
    std::vector<double> dist(graph.first.size()-1, INFINITY);
    std::vector<int> parent(graph.first.size()-1, -1);
    BinaryHeap Q;
    // init source node and add into aPQ
    parent[source] = source;
    dist[source] = 0.0;

    Q.insert(&nodeArray[source], 0.0);
    nodeArray[source].setVisited(true);

    // loop
    while(!Q.isEmpty()){
        // Delete Min, u is closest unexplored node
        Node* u = Q.deleteMin();
        // scan u, index := Index to find Neighbor Nodes in E
        int index = graph.first[u->getIndex()];
        while(index < graph.first[u->getIndex()+1]){
            // v := neighbor node of u
            Node* v = &nodeArray[graph.second[index]];
            // cancel Dijkstra if we v is our target node
            if (v->getIndex() == target){
                double d = dist[u->getIndex()] + eukld(u, v);
                dist[v->getIndex()] = d;
                parent[v->getIndex()] = u->getIndex();
                nodeArray[v->getIndex()].setVisited(true);

                std::cout << "Target found!\nDistance: "<< d << std::endl;
                return std::make_pair(dist, parent);
            }
            double d = dist[u->getIndex()] + eukld(u, v);
            if (d < dist[v->getIndex()] ){
                // new shortest distance for v, update distance and parent array
                dist[v->getIndex()] = d;
                parent[v->getIndex()] = u->getIndex();
                if (!nodeArray[v->getIndex()].isVisited()){
                    // v reached for first time, add to aPQ
                    Q.insert(v, d);
                    nodeArray[v->getIndex()].setVisited(true);
                } else {
                    // v already was in aPQ, update priority key in aPQ
                    Q.decreasePriority(v, d);
                }
            }
            // raise Index to find next Neighbor Node in E
            index++;
        }
    }
    std::cout << "Target not found." << std::endl;
    return std::make_pair(dist, parent);
}

void printParentPath (const std::pair<std::vector<double>, std::vector<int>>& distanceAndParent, int source, int target){
    int index = target-1;
    while (distanceAndParent.second[index] != source-1){
        std::cout << "Parent of "<< index << " is "<< distanceAndParent.second[index] + 1 << " with distance " << distanceAndParent.first[index] << " apart."<< std::endl;
        index = distanceAndParent.second[index];
    }
}
