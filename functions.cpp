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
std::pair<std::pair<std::vector<int>, std::vector<int>>, std::vector<std::vector<std::pair<int, int>>>>  createAdjArr(const std::vector<Edge>& EdgeList, int n, int m){
    // init V to be only 0's
    // init E with 2*m for case of undirected Graph
    std::vector<int> V(n+1, 0);
    std::vector<int> E(2*m);
    std::vector<std::vector<std::pair<int, int>>> edgeIndices(n);

    // count how many edges each node has and give each edge an Index
    int i=0;
    for (const auto& edge: EdgeList){
        V[edge.source]++;

        // We use undirected Graphs, so for ArcFlags we will only take one directed edge per undirected edge
        if (edge.source < edge.destination){
        edgeIndices[edge.source].push_back(std::make_pair(edge.destination, i++));
        }
    }

    // prefix sums for V
    for (int i = 1; i<n+1; i++){
        V[i]+=V[i-1];
    }

    // init E
    for (const auto& edge: EdgeList){
        E[--V[edge.source]] = edge.destination;
    }

    return std::make_pair(std::make_pair(V, E), edgeIndices);
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

// Set all visited of nodeArray to false
void allVisitedToFalse (std::vector<Node>& nodeArray){
    for (auto& node : nodeArray){
        node.setVisited(false);
    }
}

// Dijkstra
std::pair<std::vector<double>, std::vector<int>> Dijkstra (int source, int target, const std::pair<std::vector<int>, std::vector<int>>& graph, std::vector<Node>& nodeArray, std::vector<std::vector<std::pair<int, int>>> edgeIndices, std::vector<bool> arcFlags, int k){
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

    int partIndex = nodeArray[target].getPartition();
    int edgeIndex, index;
    // loop
    while(!Q.isEmpty()){
        // Delete Min, u is closest unexplored node
        Node* u = Q.deleteMin();
        // Cancel Dijkstra when we were to explore target node
        if (u->getIndex() == target){
            std::cout << "Target found!\nDistance: "<< dist[target] << std::endl;
            return std::make_pair(dist, parent);
        }
        // scan u, index := Index to find Neighbor Nodes in E
        index = graph.first[u->getIndex()];
        while(index < graph.first[u->getIndex()+1]){
            // v := neighbor node of u
            Node* v = &nodeArray[graph.second[index]];
            // Find Index of edge(u,v)
            if (u < v){
                for(auto pair : edgeIndices[u->getIndex()]){
                    if (pair.first == v->getIndex()){
                        edgeIndex = (pair.second * k) + partIndex;
                        break;
                    }
                }
            } else {
                for(auto pair : edgeIndices[v->getIndex()]){
                    if (pair.first == u->getIndex()){
                        edgeIndex = (pair.second * k) + partIndex;
                        break;
                    }
                }
            }

            double d = dist[u->getIndex()] + eukld(u, v);
            if ((d < dist[v->getIndex()]) &&  arcFlags[edgeIndex] ){
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
std::vector<int> modifiedDijkstra (int source, const std::pair<std::vector<int>, std::vector<int>>& graph, std::vector<Node>& nodeArray){
    // Init Parent Array, Distance Array and aPQ
    std::vector<double> dist(graph.first.size()-1, INFINITY);
    std::vector<int> parent(graph.first.size()-1, -1);
    BinaryHeap Q;
    // init source node and add into aPQ
    // in this function, the Node Indices already start at 0, so no need to do -1 (compare to other Dijkstra)
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
    return parent;
}

// Sichern der ArcFlags, sodass man nicht jedes mal neu berechnen muss
void saveArcFlags(std::vector<bool> arcFlags, int m, int k, std::string filename){
    std::string path = filename + ".arcfl";
    std::ofstream outputFile(path);

    if (outputFile.is_open()){
        for (int i = 0; i < m; i++){
            for (int j = 0; j < k; j++){
                outputFile << arcFlags[(i*k)+j];
            }
            outputFile << std::endl;
        }
    }

    outputFile.close();
    std::cout << "Data has been written to " << path << "." << std::endl;

}

std::vector<bool> readArcFlags(std::string filename, int m, int k){
    std::vector<bool> arcFlags(k*m);
    std::cout << "Reading ArcFlags file..." << std::endl;
    std::string line;
    std::ifstream MyReadFile;
    MyReadFile.open(filename);

    if (!MyReadFile){
        std::cout << "Couldn't read arcflags file " << filename << ", try providing full path." << std::endl;
        return arcFlags;
    }

    int i = 0;
    int j;
    while (getline (MyReadFile, line)){
        j = 0;
        for (char c : line){
            arcFlags[(i*k)+j] = c;
            j++;
        }
        i++;
    }

    return arcFlags;

}

// Berechnen der Arcflags
std::vector<bool> computeArcFlags(const std::pair<std::vector<int>, std::vector<int>>& graph, std::vector<Node>& nodeArray, std::vector<std::vector<std::pair<int, int>>> edgeIndices, int m, int k){
    std::cout << "Computing ArcFlags..." << std::endl;
    // Create ArcFlags Boolean Vector of Size m*k (n = nmbr of edges, k = partition size)
    // Entries m*0 to m*k are the k ArcFlags of the Edge m
    std::vector<bool> arcFlags(m*k);

    // Iterate through all Edges
    for (int u = 0; u < graph.first.size()-1; u++){
        int start = graph.first[u];
        int end = graph.first[u+1];
        for (int j = start; j < end; j++){
            int v = graph.second[j];
            // All Edges in a Partition have the ArcFlag of that partition set to true
            // (since we use Euklidian distance, the edge between two nodes is always going to be the shortest path between them)
            int partIndex = nodeArray[u].getPartition();
            if (partIndex == nodeArray[v].getPartition()){
                // Find Index of v by going through the Edges of u
                if (u < v){
                    for(auto pair : edgeIndices[u]){
                        if (pair.first == v){
                            arcFlags[(pair.second * k) + partIndex] = 1;
                            break;
                        }
                    }
                } else {
                    for(auto pair : edgeIndices[v]){
                        if (pair.first == u){
                            arcFlags[(pair.second * k) + partIndex] = 1;
                            break;
                        }
                    }
                }
            }
            // If the nodes of an Edge are not in the same partition, both are a boundary node, on which we need to perform Dijkstra later on
            else{
                nodeArray[u].setBoundary(1);
                nodeArray[v].setBoundary(1);
            }
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
    std::vector<int> parent(graph.first.size()-1);
    for (auto& node :  nodeArray){
        if (node.isBoundary()){
            allVisitedToFalse(nodeArray);
            parent = modifiedDijkstra(node.getIndex(), graph, nodeArray);
            int partIndex = node.getPartition();
            // Extract tree edges from parent array
            for (int u = 0; u < parent.size(); u++){
                // Find Index of v by going through the Edges of u
                if (u == parent[u]){
                    //std::cout << u  << " " << parent.size() << std::endl;
                    continue;
                } else if (u < parent[u]){
                    std::cout << u  << " " << parent[u] << std::endl;
                    for(auto pair : edgeIndices[u]){
                        if (pair.first == parent[u]){
                            arcFlags[(pair.second * k) + partIndex] = 1;
                            break;
                        }
                    }
                } else {
                    std::cout << u  << " " << parent[u] << std::endl;
                    for(auto pair : edgeIndices[parent[u]]){
                        if (pair.first == u){
                            arcFlags[(pair.second * k) + partIndex] = 1;
                            break;
                        }
                    }
                }
            }
        std::cout << "Node " << i++ << " done!" << std::endl;
        }
    }
    return arcFlags;
}
