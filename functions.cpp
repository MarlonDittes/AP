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
#include <omp.h>
#include <iomanip>

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

void saveExploredNodes(std::vector<int> exploredNodes, std::string filename, std::vector<Node>& nodeArray){
    std::cout << "Saving Explored Nodes..." << std::endl;
    std::string path = filename;
    std::ofstream outputFile(path);

    if (!outputFile.is_open()){
        std::cout << "Couldn't create explored nodes file " << filename << "." << std::endl;
        return;
    }

    outputFile << std::fixed << std::setprecision(4);

    for (auto& n : exploredNodes){
        outputFile << std::fixed << nodeArray[n].getX() << " " << nodeArray[n].getY() << std::endl;
    }

    outputFile.close();
    std::cout << "Data has been written to " << path << "." << std::endl;
    return;
}

// Distanz Berechnung mittels Euklidischer Norm
double eukld(Node* u, Node* v) {
    double norm = 0.0;
    norm += std::pow(u->getX() - v->getX(), 2.0);
    norm += std::pow(u->getY() - v->getY(), 2.0);
    return std::sqrt(norm);
}

//Berechne Distanz für alle Edges im Graph
void computeDistances(std::pair<std::vector<int>, std::vector<Edge*>>& graph, std::vector<Node>& nodeArray){
    //Iterate through all Edges
    for (int i = 0; i < graph.second.size(); i++){
        graph.second[i]->distance = eukld(&nodeArray[graph.second[i]->source], &nodeArray[graph.second[i]->destination]);
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
    //std::cout << "Performing Djikstra..." << std::endl;
    assert(source > 0 and target > 0 and source != target);
    source--;
    target--;
    // Create distance, parent Array and aPQ
    // default distance is Infinity
    // default parent is bottom (here nullptr)
    std::vector<double> dist(graph.first.size()-1, INFINITY);
    std::vector<Edge*> parent(graph.first.size()-1, nullptr);
    // For Plotting:
    //std::vector<int> exploredNodes;

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

        // For Plotting:
        //exploredNodes.push_back(uInd);

        // Cancel Dijkstra when we were to explore target node
        if (uInd == target){
            // For Plotting:
            //saveExploredNodes(exploredNodes, "../plot/Dijkstra" + std::to_string(source+1) + "to" + std::to_string(target+1), nodeArray);

            //std::cout << "Target found!\nDistance: "<< dist[target] << std::endl;
            return std::make_pair(dist, parent);
        }
        // scan u, index := Index to find Neighbor Nodes in E
        index = graph.first[uInd];
        while(index < graph.first[uInd+1]){
            // v := neighbor node of u
            Node* v = &nodeArray[graph.second[index]->getDestination(uInd)];
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
    //std::cout << "Target not found." << std::endl;
    return std::make_pair(dist, parent);
}

// A-Star Dijkstra
std::pair<std::vector<double>, std::vector<Edge*>> AStarDijkstra (int source, int target, std::pair<std::vector<int>, std::vector<Edge*>>& graph, std::vector<Node>& nodeArray){
    // Source, Target node index -1 so it fits into the data structure
    //std::cout << "Performing AStarDjikstra..." << std::endl;
    assert(source > 0 and target > 0 and source != target);
    source--;
    target--;
    // Create distance, parent Array and aPQ
    // default distance is Infinity
    // default parent is bottom (here nullptr)
    std::vector<double> dist(graph.first.size()-1, INFINITY);
    std::vector<Edge*> parent(graph.first.size()-1, nullptr);
    // For Plotting:
    //std::vector<int> exploredNodes;

    BinaryHeap Q;
    // init source node and add into aPQ
    parent[source] = nullptr;
    dist[source] = 0.0;

    int potentialSource = eukld(&nodeArray[source], &nodeArray[target]);
    Q.insert(&nodeArray[source], 0.0 + potentialSource);
    nodeArray[source].setVisited(true);

    int index;
    // loop
    while(!Q.isEmpty()){
        // Delete Min, u is closest unexplored node
        Node* u = Q.deleteMin();
        int uInd = u->getIndex();

        // For Plotting:
        //exploredNodes.push_back(uInd);

        // Cancel Dijkstra when we were to explore target node
        if (uInd == target){
            // For Plotting:
            //saveExploredNodes(exploredNodes, "../plot/AStarDijkstra" + std::to_string(source+1) + "to" + std::to_string(target+1), nodeArray);

            //std::cout << "Target found!\nDistance: "<< dist[target] << std::endl;
            return std::make_pair(dist, parent);
        }
        // scan u, index := Index to find Neighbor Nodes in E
        index = graph.first[uInd];
        while(index < graph.first[uInd+1]){
            // v := neighbor node of u
            Node* v = &nodeArray[graph.second[index]->getDestination(uInd)];
            int vInd = v->getIndex();
            double d = dist[uInd] + graph.second[index]->distance;

            if (d < dist[vInd]){
                // new shortest distance for v, update distance and parent array
                dist[vInd] = d;
                parent[vInd] = graph.second[index];
                int potentialV = eukld(v, &nodeArray[target]);
                if (!nodeArray[vInd].isVisited()){
                    // v reached for first time, add to aPQ
                    Q.insert(v, d + potentialV);
                    nodeArray[vInd].setVisited(true);
                } else {
                    // v already was in aPQ, update priority key in aPQ
                    Q.decreasePriority(v, d + potentialV);
                }
            }
            // raise Index to find next Neighbor Node in E
            index++;
        }
    }
    //std::cout << "Target not found." << std::endl;
    return std::make_pair(dist, parent);
}

// Arc-Flags Dijkstra
std::pair<std::vector<double>, std::vector<Edge*>> ArcFlagsDijkstra (int source, int target, std::pair<std::vector<int>, std::vector<Edge*>>& graph, std::vector<Node>& nodeArray){
    // Source, Target node index -1 so it fits into the data structure
    //std::cout << "Performing ArcFlags Djikstra..." << std::endl;
    assert(source > 0 and target > 0);
    source--;
    target--;
    // Create distance, parent Array and aPQ
    // default distance is Infinity
    // default parent is bottom (here nullptr)
    std::vector<double> dist(graph.first.size()-1, INFINITY);
    std::vector<Edge*> parent(graph.first.size()-1, nullptr);
    // For Plotting:
    //std::vector<int> exploredNodes;

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

        // For Plotting:
        //exploredNodes.push_back(uInd);

        // Cancel Dijkstra when we were to explore target node
        if (uInd == target){
            // For Plotting:
            //saveExploredNodes(exploredNodes, "../plot/ArcFlagsDijkstra" + std::to_string(source+1) + "to" + std::to_string(target+1), nodeArray);

            //std::cout << "Target found!\nDistance: "<< dist[target] << std::endl;
            return std::make_pair(dist, parent);
        }
        // scan u, index := Index to find Neighbor Nodes in E
        index = graph.first[uInd];
        while(index < graph.first[uInd+1]){
            // v := neighbor node of u
            Node* v = &nodeArray[graph.second[index]->getDestination(uInd)];
            int vInd = v->getIndex();
            double d = dist[uInd] + graph.second[index]->distance;

            //Check if shorter AND ArcFlag is set
            if ((d < dist[vInd]) && graph.second[index]->getArcFlag(uInd, partIndex)){
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
    //std::cout << "Target not found." << std::endl;
    return std::make_pair(dist, parent);
}

// A-Star Arc-Flags Dijkstra
std::pair<std::vector<double>, std::vector<Edge*>> AStarArcFlagsDijkstra (int source, int target, std::pair<std::vector<int>, std::vector<Edge*>>& graph, std::vector<Node>& nodeArray){
    // Source, Target node index -1 so it fits into the data structure
    //std::cout << "Performing AStarArcFlags Djikstra..." << std::endl;
    assert(source > 0 and target > 0);
    source--;
    target--;
    // Create distance, parent Array and aPQ
    // default distance is Infinity
    // default parent is bottom (here nullptr)
    std::vector<double> dist(graph.first.size()-1, INFINITY);
    std::vector<Edge*> parent(graph.first.size()-1, nullptr);
    // For Plotting:
    //std::vector<int> exploredNodes;

    BinaryHeap Q;
    // init source node and add into aPQ
    parent[source] = nullptr;
    dist[source] = 0.0;

    int potentialSource = eukld(&nodeArray[source], &nodeArray[target]);
    Q.insert(&nodeArray[source], 0.0 + potentialSource);
    nodeArray[source].setVisited(true);

    int partIndex = nodeArray[target].getPartition();
    int index;
    // loop
    while(!Q.isEmpty()){
        // Delete Min, u is closest unexplored node
        Node* u = Q.deleteMin();
        int uInd = u->getIndex();

        // For Plotting:
        //exploredNodes.push_back(uInd);

        // Cancel Dijkstra when we were to explore target node
        if (uInd == target){
            // For Plotting:
            //saveExploredNodes(exploredNodes, "../plot/AStarArcFlagsDijkstra" + std::to_string(source+1) + "to" + std::to_string(target+1), nodeArray);

            //std::cout << "Target found!\nDistance: "<< dist[target] << std::endl;
            return std::make_pair(dist, parent);
        }
        // scan u, index := Index to find Neighbor Nodes in E
        index = graph.first[uInd];
        while(index < graph.first[uInd+1]){
            // v := neighbor node of u
            Node* v = &nodeArray[graph.second[index]->getDestination(uInd)];
            int vInd = v->getIndex();
            double d = dist[uInd] + graph.second[index]->distance;

            //Check if shorter AND ArcFlag is set
            if ((d < dist[vInd]) && graph.second[index]->getArcFlag(uInd, partIndex)){
                // new shortest distance for v, update distance and parent array
                dist[vInd] = d;
                parent[vInd] = graph.second[index];
                int potentialV = eukld(v, &nodeArray[target]);
                if (!nodeArray[vInd].isVisited()){
                    // v reached for first time, add to aPQ
                    Q.insert(v, d + potentialV);
                    nodeArray[vInd].setVisited(true);
                } else {
                    // v already was in aPQ, update priority key in aPQ
                    Q.decreasePriority(v, d + potentialV);
                }
            }
            // raise Index to find next Neighbor Node in E
            index++;
        }
    }
    //std::cout << "Target not found." << std::endl;
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
            Node* v = &nodeArray[graph.second[index]->getDestination(uInd)];
            int vInd = v->getIndex();
            double d = dist[uInd] + graph.second[index]->distance;
            if (d < dist[vInd] ){
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
    return parent;
}

void initEdgeArcflags (std::pair<std::vector<int>, std::vector<Edge*>>& graph, int k){
    //Iterate through all Edges
    for (int i = 0; i < graph.second.size(); i++){
        //graph.second[i]->arcFlagsBackwards.resize(k);
        //graph.second[i]->arcFlagsForwards.resize(k);
        //Init all to 0
        for (int j = 0; j < k; j++){
            graph.second[i]->arcFlagsBackwards[j] = 0;
            graph.second[i]->arcFlagsForwards[j] = 0;
        }
    }
}

// Sichern der ArcFlags, sodass man nicht jedes mal neu berechnen muss
void saveArcFlags(std::pair<std::vector<int>, std::vector<Edge*>>& graph, int k, std::string filename){
    std::cout << "Saving ArcFlags..." << std::endl;
    std::string path = filename + ".arcfl";
    std::ofstream outputFile(path);

    if (!outputFile.is_open()){
        std::cout << "Couldn't create arcflags file " << filename << "." << std::endl;
        return;
    }

    for (int i = 0; i < graph.first.size(); i++){
        for (int j = graph.first[i]; j < graph.first[i+1]; j++){
            for (int n = 0; n < k; n++){
                outputFile << graph.second[j]->getArcFlag(i, n);
            }
            outputFile << std::endl;
        }
    }

    outputFile.close();
    std::cout << "Data has been written to " << path << "." << std::endl;
    return;
}

void readArcFlags(std::pair<std::vector<int>, std::vector<Edge*>>& graph, std::string filename){
    std::cout << "Reading ArcFlags file..." << std::endl;
    std::string line;
    std::ifstream MyReadFile;
    MyReadFile.open(filename);

    if (!MyReadFile){
        std::cout << "Couldn't read arcflags file " << filename << ", try providing full path." << std::endl;
        return;
    }

    for (int i = 0; i < graph.first.size(); i++){
        for (int j = graph.first[i]; j < graph.first[i+1]; j++){
            getline (MyReadFile, line);
            int k = 0;
            for (char c : line){
                graph.second[j]->setBackwardArcFlag(i, k, (c == '1'));
                k++;
            }
        }
    }

    return;
}

void computeArcFlags(std::vector<Edge>& EdgeList, std::pair<std::vector<int>, std::vector<Edge*>>& graph, std::vector<Node>& nodeArray, int n){
    std::cout << "Computing ArcFlags..." << std::endl;

    // Iterate through all Edges
    for (auto& edge : EdgeList){
        int partIndex = nodeArray[edge.source].getPartition();
        if (partIndex == nodeArray[edge.destination].getPartition()){
            edge.arcFlagsBackwards[partIndex] = 1;
            edge.arcFlagsForwards[partIndex] = 1;
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
            if (parent[u] != nullptr){
                // Save Flag on backwards Edge
                parent[u]->setBackwardArcFlag(u, partIndex, 1);
            }
        }
        std::cout << "Node " << i++ << " done!" << std::endl;
    }

    return;
}

void parallelComputeArcFlags(std::vector<Edge>& EdgeList, std::pair<std::vector<int>, std::vector<Edge*>>& graph, std::vector<Node>& nodeArray, int n){
    std::cout << "Computing ArcFlags..." << std::endl;

    // Iterate through all Edges
    for (auto& edge : EdgeList){
        int partIndex = nodeArray[edge.source].getPartition();
        if (partIndex == nodeArray[edge.destination].getPartition()){
            edge.arcFlagsBackwards[partIndex] = 1;
            edge.arcFlagsForwards[partIndex] = 1;
        } else {
            nodeArray[edge.source].setBoundary(1);
            nodeArray[edge.destination].setBoundary(1);
        }
    }

    // Count boundary nodes for output (can comment this out for slightly quicker performance)
    int count = 0;
    for (auto& node : nodeArray){
        if (node.isBoundary()){
            count++;
        }
    }
    std::cout << "Performing (backwards) Dijkstra on " << count << " boundary nodes..." << std::endl;


    // Declare the counter variable as shared
    int sharedCount = 0;

    #pragma omp parallel shared(sharedCount)
    {
        // Now run (backwards) Dijkstra on all nodes we marked as boundary
        #pragma omp for
        for (int i = 0; i < nodeArray.size(); i++) {
            auto& node = nodeArray[i];
            if (!node.isBoundary()) {
                continue;
            }
            auto nodeArrayCopy = nodeArray;
            allVisitedToFalse(nodeArrayCopy);
            auto parent = modifiedDijkstra(i, graph, nodeArrayCopy);
            int partIndex = node.getPartition();

            // Extract tree edges from parent array
            for (int u = 0; u < parent.size(); u++) {
                // Find Index of v by going through the Edges of u
                if (parent[u] != nullptr) {
                    // Lock the update of arcFlags
                    // Save Flag on backwards Edge
                    #pragma omp critical
                    {
                        parent[u]->setBackwardArcFlag(u, partIndex, 1);
                    }
                }
            }

            // Increase the shared count
            #pragma omp atomic
            sharedCount++;

            // Output the current count of completed nodes
            if (sharedCount % 10 == 0){
                #pragma omp critical
                {
                std::cout << "Nodes completed so far: " << sharedCount << " of " << count << std::endl;
                }
            }
        }
    }

    // Output the final count
    std::cout << "Total nodes completed: " << sharedCount << std::endl;

    return;
}

std::vector<std::pair<int, int>> readTestFile (std::string filename){
    std::cout << "Reading Test file..." << std::endl;
    std::vector<std::pair<int, int>> testPairs;
    std::string line;
    std::ifstream MyReadFile;
    MyReadFile.open(filename);

    if (!MyReadFile){
        std::cout << "Couldn't read test file " << filename << ", try providing full path." << std::endl;
        return testPairs;
    }

    int source, target;
    while (getline (MyReadFile, line)){
        std::istringstream iss(line);
        iss >> source;
        iss >> target;
        testPairs.push_back(std::make_pair(source, target));
    }

    return testPairs;
}

void saveResults(std::vector<std::pair<int, long long>> searchSpaceAndTime, std::string filename, int k, std::string type){
    std::cout << "Saving Test Results..." << std::endl;
    std::string path = filename + type + std::to_string(k);
    std::ofstream outputFile(path);

    if (!outputFile.is_open()){
        std::cout << "Couldn't create test result file " << filename << "." << std::endl;
        return;
    }

    for (auto& pair : searchSpaceAndTime){
        outputFile << pair.first << " " << pair.second << std::endl;
    }

    outputFile.close();
    std::cout << "Data has been written to " << path << "." << std::endl;
    return;
}

// Search Space Returning
// Dijkstra
int searchSpaceDijkstra (int source, int target, std::pair<std::vector<int>, std::vector<Edge*>>& graph, std::vector<Node>& nodeArray){
    // Source, Target node index -1 so it fits into the data structure
    assert(source > 0 and target > 0 and source != target);
    source--;
    target--;
    // Create distance, parent Array and aPQ
    // default distance is Infinity
    // default parent is bottom (here nullptr)
    std::vector<double> dist(graph.first.size()-1, INFINITY);
    std::vector<Edge*> parent(graph.first.size()-1, nullptr);

    BinaryHeap Q;
    // init source node and add into aPQ
    parent[source] = nullptr;
    dist[source] = 0.0;

    Q.insert(&nodeArray[source], 0.0);
    nodeArray[source].setVisited(true);

    int index;
    int searchSpace = -1;
    // loop
    while(!Q.isEmpty()){
        searchSpace++;
        // Delete Min, u is closest unexplored node
        Node* u = Q.deleteMin();
        int uInd = u->getIndex();

        // Cancel Dijkstra when we were to explore target node
        if (uInd == target){
            return searchSpace;
        }
        // scan u, index := Index to find Neighbor Nodes in E
        index = graph.first[uInd];
        while(index < graph.first[uInd+1]){
            // v := neighbor node of u
            Node* v = &nodeArray[graph.second[index]->getDestination(uInd)];
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
    return 0;
}

// A-Star Dijkstra
int searchSpaceAStarDijkstra (int source, int target, std::pair<std::vector<int>, std::vector<Edge*>>& graph, std::vector<Node>& nodeArray){
    // Source, Target node index -1 so it fits into the data structure
    assert(source > 0 and target > 0 and source != target);
    source--;
    target--;
    // Create distance, parent Array and aPQ
    // default distance is Infinity
    // default parent is bottom (here nullptr)
    std::vector<double> dist(graph.first.size()-1, INFINITY);
    std::vector<Edge*> parent(graph.first.size()-1, nullptr);

    BinaryHeap Q;
    // init source node and add into aPQ
    parent[source] = nullptr;
    dist[source] = 0.0;

    int potentialSource = eukld(&nodeArray[source], &nodeArray[target]);
    Q.insert(&nodeArray[source], 0.0 + potentialSource);
    nodeArray[source].setVisited(true);

    int index;
    int searchSpace = -1;
    // loop
    while(!Q.isEmpty()){
        searchSpace++;
        // Delete Min, u is closest unexplored node
        Node* u = Q.deleteMin();
        int uInd = u->getIndex();

        // Cancel Dijkstra when we were to explore target node
        if (uInd == target){
            return searchSpace;
        }
        // scan u, index := Index to find Neighbor Nodes in E
        index = graph.first[uInd];
        while(index < graph.first[uInd+1]){
            // v := neighbor node of u
            Node* v = &nodeArray[graph.second[index]->getDestination(uInd)];
            int vInd = v->getIndex();
            double d = dist[uInd] + graph.second[index]->distance;

            if (d < dist[vInd]){
                // new shortest distance for v, update distance and parent array
                dist[vInd] = d;
                parent[vInd] = graph.second[index];
                int potentialV = eukld(v, &nodeArray[target]);
                if (!nodeArray[vInd].isVisited()){
                    // v reached for first time, add to aPQ
                    Q.insert(v, d + potentialV);
                    nodeArray[vInd].setVisited(true);
                } else {
                    // v already was in aPQ, update priority key in aPQ
                    Q.decreasePriority(v, d + potentialV);
                }
            }
            // raise Index to find next Neighbor Node in E
            index++;
        }
    }
    return 0;
}

// Arc-Flags Dijkstra
int searchSpaceArcFlagsDijkstra (int source, int target, std::pair<std::vector<int>, std::vector<Edge*>>& graph, std::vector<Node>& nodeArray){
    // Source, Target node index -1 so it fits into the data structure
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
    int searchSpace = -1;
    // loop
    while(!Q.isEmpty()){
        searchSpace++;
        // Delete Min, u is closest unexplored node
        Node* u = Q.deleteMin();
        int uInd = u->getIndex();

        // Cancel Dijkstra when we were to explore target node
        if (uInd == target){
            return searchSpace;
        }
        // scan u, index := Index to find Neighbor Nodes in E
        index = graph.first[uInd];
        while(index < graph.first[uInd+1]){
            // v := neighbor node of u
            Node* v = &nodeArray[graph.second[index]->getDestination(uInd)];
            int vInd = v->getIndex();
            double d = dist[uInd] + graph.second[index]->distance;

            //Check if shorter AND ArcFlag is set
            if ((d < dist[vInd]) && graph.second[index]->getArcFlag(uInd, partIndex)){
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
    return 0;
}

// A-Star Arc-Flags Dijkstra
int searchSpaceAStarArcFlagsDijkstra (int source, int target, std::pair<std::vector<int>, std::vector<Edge*>>& graph, std::vector<Node>& nodeArray){
    // Source, Target node index -1 so it fits into the data structure
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

    int potentialSource = eukld(&nodeArray[source], &nodeArray[target]);
    Q.insert(&nodeArray[source], 0.0 + potentialSource);
    nodeArray[source].setVisited(true);

    int partIndex = nodeArray[target].getPartition();
    int index;
    int searchSpace = -1;
    // loop
    while(!Q.isEmpty()){
        searchSpace++;
        // Delete Min, u is closest unexplored node
        Node* u = Q.deleteMin();
        int uInd = u->getIndex();

        // Cancel Dijkstra when we were to explore target node
        if (uInd == target){
            return searchSpace;
        }
        // scan u, index := Index to find Neighbor Nodes in E
        index = graph.first[uInd];
        while(index < graph.first[uInd+1]){
            // v := neighbor node of u
            Node* v = &nodeArray[graph.second[index]->getDestination(uInd)];
            int vInd = v->getIndex();
            double d = dist[uInd] + graph.second[index]->distance;

            //Check if shorter AND ArcFlag is set
            if ((d < dist[vInd]) && graph.second[index]->getArcFlag(uInd, partIndex)){
                // new shortest distance for v, update distance and parent array
                dist[vInd] = d;
                parent[vInd] = graph.second[index];
                int potentialV = eukld(v, &nodeArray[target]);
                if (!nodeArray[vInd].isVisited()){
                    // v reached for first time, add to aPQ
                    Q.insert(v, d + potentialV);
                    nodeArray[vInd].setVisited(true);
                } else {
                    // v already was in aPQ, update priority key in aPQ
                    Q.decreasePriority(v, d + potentialV);
                }
            }
            // raise Index to find next Neighbor Node in E
            index++;
        }
    }
    return 0;
}
