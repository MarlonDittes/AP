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
#include <tuple>

// Create Adjacency Array using an EdgeList
std::pair<std::vector<int>, std::vector<Edge*>>  createAdjArr(std::vector<Edge>& EdgeList, int n, int m){
    // init V to be only 0's
    // init E with 2*m for case of undirected Graph
    std::vector<int> V(n+1, 0);
    std::vector<Edge*> E(2*m);

    // count how many edges each node has and give each edge an Index
    for (const auto& edge: EdgeList){
        V[edge.source]++;
        V[edge.destination]++;
    }

    // prefix sums for V
    for (int i = 1; i<n+1; i++){
        V[i]+=V[i-1];
    }

    // init E and save Indices
    for (auto& edge: EdgeList){
        E[--V[edge.source]] = &edge;
        E[--V[edge.destination]] = &edge;
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
std::tuple<std::vector<Edge>, int, int> readGraphFile (std::string filename){
    std::cout << "Reading Graph file..." << std::endl;
    std::string line;
    std::ifstream MyReadFile;
    MyReadFile.open(filename);
    std::vector<Edge> EdgeList;

    if (!MyReadFile){
        std::cout << "Couldn't read graph file " << filename << ", try providing full path." << std::endl;
        return std::make_tuple(EdgeList, 0, 0);
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
            // Only save forward edges
            if(u<v){
                Edge e(u,v);
                EdgeList.push_back(e);
            }
        }
        u++;
    }

    return std::make_tuple(EdgeList, n, m);
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

//Berechne Distanz für alle Edges im Graph
void computeDistances(std::vector<Edge>& EdgeList, std::vector<Node>& nodeArray){
    for (auto& edge : EdgeList){
        edge.distance = eukld(&nodeArray[edge.source], &nodeArray[edge.destination]);
    }
}

// Set all visited of nodeArray to false
void allVisitedToFalse (std::vector<Node>& nodeArray){
    for (auto& node : nodeArray){
        node.setVisited(false);
    }
}

// Dijkstra
std::pair<std::vector<double>, std::vector<Edge*>> Dijkstra (int source, int target, std::pair<std::vector<int>, std::vector<Edge*>>& graph, std::vector<Node>& nodeArray){
    // Source, Target node index -1 so it fits into the data structure
    std::cout << "Performing Djikstra..." << std::endl;
    assert(source > 0 and target > 0 and source != target);
    source--;
    target--;
    // Create distance, parent Array and aPQ
    // default distance is Infinity
    // default parent is bottom (here -1)
    std::vector<double> dist(graph.first.size()-1, INFINITY);
    std::vector<Edge*> parent(graph.first.size()-1, nullptr);
    BinaryHeap Q;
    // init source node and add into aPQ
    parent[source] = nullptr;
    dist[source] = 0.0;

    Q.insert(&nodeArray[source], 0.0);
    nodeArray[source].setVisited(true);

    int index;
    // loop
    while(!Q.isEmpty()){
        // Delete Min, u is closest unexplored node
        Node* u = Q.deleteMin();
        int uInd = u->getIndex();
        // Cancel Dijkstra when we were to explore target node
        if (uInd == target){
            std::cout << "Target found!\nDistance: "<< dist[target] << std::endl;
            return std::make_pair(dist, parent);
        }
        // scan u, index := Index to find Neighbor Nodes in E
        index = graph.first[uInd];
        while(index < graph.first[uInd+1]){
            // v := neighbor node of u
            Node* v = &nodeArray[graph.second[index]->other(uInd)];
            int vInd = v->getIndex();
            double d = dist[uInd] + graph.second[index]->distance;

            if (d < dist[vInd]){
                // new shortest distance for v, update distance and parent array
                dist[vInd] = d;
                parent[vInd] = graph.second[index];
                if (!nodeArray[vInd].isVisited()){
                    // v reached for first time, add to aPQ
                    Q.insert(v, d);
                    nodeArray[vInd].setVisited(true);
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

// Arc-Flags Dijkstra
std::pair<std::vector<double>, std::vector<Edge*>> ArcFlagsDijkstra (int source, int target, std::pair<std::vector<int>, std::vector<Edge*>>& graph, std::vector<Node>& nodeArray){
    // Source, Target node index -1 so it fits into the data structure
    std::cout << "Performing ArcFlags Djikstra..." << std::endl;
    assert(source > 0 and target > 0);
    source--;
    target--;
    // Create distance, parent Array and aPQ
    // default distance is Infinity
    // default parent is bottom (here -1)
    std::vector<double> dist(graph.first.size()-1, INFINITY);
    std::vector<Edge*> parent(graph.first.size()-1, nullptr);
    BinaryHeap Q;
    // init source node and add into aPQ
    parent[source] = nullptr;
    dist[source] = 0.0;

    Q.insert(&nodeArray[source], 0.0);
    nodeArray[source].setVisited(true);

    int partIndex = nodeArray[target].getPartition();
    int index;
    // loop
    while(!Q.isEmpty()){
        // Delete Min, u is closest unexplored node
        Node* u = Q.deleteMin();
        int uInd = u->getIndex();
        // Cancel Dijkstra when we were to explore target node
        if (uInd == target){
            std::cout << "Target found!\nDistance: "<< dist[target] << std::endl;
            return std::make_pair(dist, parent);
        }
        // scan u, index := Index to find Neighbor Nodes in E
        index = graph.first[uInd];
        while(index < graph.first[uInd+1]){
            // v := neighbor node of u
            Node* v = &nodeArray[graph.second[index]->other(uInd)];
            int vInd = v->getIndex();
            double d = dist[uInd] + graph.second[index]->distance;

            if ((d < dist[vInd]) &&  graph.second[index]->arcFlags[partIndex]){
                // new shortest distance for v, update distance and parent array
                dist[vInd] = d;
                parent[vInd] = graph.second[index];
                if (!nodeArray[vInd].isVisited()){
                    // v reached for first time, add to aPQ
                    Q.insert(v, d);
                    nodeArray[vInd].setVisited(true);
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

// Einlesen der mit KaHIP erstellten Partition
void readPartitionFile(std::string filename, std::vector<Node>& nodeArray){
    std::cout << "Reading Partition file..." << std::endl;
    std::string line;
    std::ifstream MyReadFile;
    MyReadFile.open(filename);

    if (!MyReadFile){
        std::cout << "Couldn't read partition file " << filename << ", try providing full path." << std::endl;
        return;
    }

    int partNr;
    int u = 0;
    while (getline (MyReadFile, line)){
        std::istringstream iss(line);
        iss >> partNr;
        nodeArray[u].setPartition(partNr);
        u++;
    }

    return;
}

// Modified Dijkstra for ArcFlags
std::vector<Edge*> modifiedDijkstra (int source, std::pair<std::vector<int>, std::vector<Edge*>>& graph, std::vector<Node>& nodeArray){
    // Init Parent Array, Distance Array and aPQ
    std::vector<double> dist(graph.first.size()-1, INFINITY);
    std::vector<Edge*> parent(graph.first.size()-1, nullptr);
    BinaryHeap Q;
    // init source node and add into aPQ
    // in this function, the Node Indices already start at 0, so no need to do -1 (compare to other Dijkstra)
    parent[source] = nullptr;
    dist[source] = 0.0;

    Q.insert(&nodeArray[source], 0.0);
    nodeArray[source].setVisited(true);

    // loop
    while(!Q.isEmpty()){
        // Delete Min, u is closest unexplored node
        Node* u = Q.deleteMin();
        int uInd = u->getIndex();

        // scan u, index := Index to find Neighbor Nodes in E
        int index = graph.first[uInd];
        while(index < graph.first[uInd+1]){
            // v := neighbor node of u
            Node* v = &nodeArray[graph.second[index]->other(uInd)];
            int vInd = v->getIndex();
            double d = dist[uInd] + graph.second[index]->distance;
            if (d < dist[vInd] ){
                // new shortest distance for v, update distance and parent array
                dist[vInd] = d;
                parent[vInd] = graph.second[index];
                if (!nodeArray[v->getIndex()].isVisited()){
                    // v reached for first time, add to aPQ
                    Q.insert(v, d);
                    nodeArray[vInd].setVisited(true);
                } else {
                    // v already was in aPQ, update priority key in aPQ
                    Q.decreasePriority(v, d);
                }
            }
            // raise Index to find next Neighbor Node in E
            index++;
        }
    }
    return parent;
}

void initEdgeArcflags (std::vector<Edge>& EdgeList, int k){
    for (auto& edge : EdgeList){
        edge.arcFlags.resize(k);
        for (int i = 0; i < k; i++){
            edge.arcFlags[i] = 0;
        }
    }
}

// Sichern der ArcFlags, sodass man nicht jedes mal neu berechnen muss
void saveArcFlags(std::vector<Edge>& EdgeList, int k, std::string filename){
    std::string path = filename + ".arcfl";
    std::ofstream outputFile(path);

    if (outputFile.is_open()){
        for (auto& edge : EdgeList){
            for (int j = 0; j < k; j++){
                outputFile << edge.arcFlags[j];
            }
            outputFile << std::endl;
        }
    }

    outputFile.close();
    std::cout << "Data has been written to " << path << "." << std::endl;

}

void readArcFlags(std::vector<Edge>& EdgeList, std::string filename){
    std::cout << "Reading ArcFlags file..." << std::endl;
    std::string line;
    std::ifstream MyReadFile;
    MyReadFile.open(filename);

    if (!MyReadFile){
        std::cout << "Couldn't read arcflags file " << filename << ", try providing full path." << std::endl;
        return;
    }

    int i = 0;
    int j;
    while (getline (MyReadFile, line)){
        j = 0;
        for (char c : line){
            EdgeList[i].arcFlags[j] = c;
            j++;
        }
        i++;
    }

    return;
}

void computeArcFlags(std::vector<Edge>& EdgeList, std::pair<std::vector<int>, std::vector<Edge*>>& graph, std::vector<Node>& nodeArray, int n){
    std::cout << "Computing ArcFlags..." << std::endl;

    // Iterate through all Edges
    for (auto& edge : EdgeList){
        int partIndex = nodeArray[edge.source].getPartition();
        // Same Partition? -> ArcFlags for this Edge on their partition = 1
        if (partIndex == nodeArray[edge.destination].getPartition()){
            edge.arcFlags[partIndex] = 1;
        } else {
            nodeArray[edge.source].setBoundary(1);
            nodeArray[edge.destination].setBoundary(1);
        }
    }

    // Count boundary nodes for output (can comment this out for slighlty quicker performance)
    int count = 0;
    for (auto& node : nodeArray){
        if (node.isBoundary()){
            count++;
        }
    }
    std::cout << "Performing (backwards) Dijkstra on " << count << " boundary nodes..." << std::endl;

    int i = 1;
    // Now run (backwards) Dijkstra on all nodes we marked as boundary
    std::vector<Edge*> parent(n, nullptr);
    for (auto& node :  nodeArray){
        if (!node.isBoundary()){
            continue;
        }

        allVisitedToFalse(nodeArray);
        parent = modifiedDijkstra(node.getIndex(), graph, nodeArray);
        int partIndex = node.getPartition();
        // Extract tree edges from parent array
        for (int u = 0; u < parent.size(); u++){
            // Find Index of v by going through the Edges of u
            if (parent[u] == nullptr){
                continue;
            } else {
                parent[u]->arcFlags[partIndex] = 1;
            }
        }
        std::cout << "Node " << i++ << " done!" << std::endl;
    }

    return;
}
