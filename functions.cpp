#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "node.h"

// Create Adjacency Array using an EdgeList
std::pair<std::vector<int>, std::vector<int>> createAdjArr(const std::vector<std::pair<int, int>>& EdgeList, int n, int m){
    // init V to be only 0's
    // init E with 2*m for case of undirected Graph
    std::vector<int> V(n+1, 0);
    std::vector<int> E(2*m);

    // count how many edges each node has
    for (const auto& edge: EdgeList){
        V[edge.first]++;
    }

    // prefix sums for V
    for (int i = 1; i<n+1; i++){
        V[i]+=V[i-1];
    }

    // init E
    for (const auto& edge: EdgeList){
        E[--V[edge.first]] = edge.second;
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
    std::string line;
    std::ifstream MyReadFile;
    MyReadFile.open(filename);
    std::vector<std::pair<int, int>> EdgeList;

    if (!MyReadFile){
        std::cout << "Couldn't read graph file " << filename << ", try providing full path." << std::endl;
        auto graph = createAdjArr(EdgeList, 0, 0);
        return graph;
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
    int u,v;
    u = 0;
    while (getline (MyReadFile, line)){
        std::istringstream iss(line);
        while (iss >> v){
            v--;
            EdgeList.push_back(std::make_pair(u, v));
        }
        u++;
    }

    // Create and return Graph using Adjacency Array
    auto graph = createAdjArr(EdgeList, n, m);
    return graph;
}

// Create NodeArray from coordinate file
std::vector<Node> readCoordFile (std::string filename){
    std::string line;
    std::ifstream MyReadFile;
    MyReadFile.open(filename);
    std::vector<Node> nodeArray;

    if (!MyReadFile){
        std::cout << "Couldn't read coordinate file " << filename << ", try providing full path." << std::endl;
        return nodeArray;
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
        Node n(i, x, y);
        nodeArray.push_back(n);
        // increase index for next line
        i++;
    }
    // return nodeArray of Nodes
    return nodeArray;
}
