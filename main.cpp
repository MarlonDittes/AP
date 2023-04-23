#include <iostream>
#include <array>
#include <utility>
#include <vector>
#include <ctime>
#include <chrono>
#include <fstream>
#include <sstream>

const size_t NODE_COUNT = 15606;
const size_t EDGE_COUNT = 45878;

// print graph (AdjArr)
void printGraph (std::pair<std::array<int, NODE_COUNT+1>, std::array<int, EDGE_COUNT>> adjArr){
    std::cout << "Printing Graph: \n";

    // print V
    std::cout << "V: ";
    std::cout << adjArr.first[0];

    for (int i = 1; i<adjArr.first.size(); i++){
        std::cout << ", " << adjArr.first[i];
    }
    std::cout << std::endl;

    // print E
    std::cout << "E: ";
    std::cout << adjArr.second[0];

    for (int i = 1; i<adjArr.second.size(); i++){
        std::cout << ", " << adjArr.second[i];
    }
    std::cout << std::endl;
}

void printGraphShort (std::pair<std::array<int, NODE_COUNT+1>, std::array<int, EDGE_COUNT>> adjArr, int length){
    std::cout << "Printing Graph: \n";

    // print V
    std::cout << "V: ";
    std::cout << adjArr.first[0];

    for (int i = 1; i<length; i++){
        std::cout << ", " << adjArr.first[i];
    }

    std::cout << std::endl;

    // print E
    std::cout << "E: ";
    std::cout << adjArr.second[0];

    for (int i = 1; i<length; i++){
        std::cout << ", " << adjArr.second[i];
    }

    std::cout << std::endl;
}

// create a graph data structure (AdjArr)
std::pair<std::array<int, NODE_COUNT+1>, std::array<int, EDGE_COUNT>> adjacencyArray (const std::vector<std::pair<int, int>>& EdgeList) {
    std::array<int, NODE_COUNT+1> V;
    std::array<int, EDGE_COUNT> E;

    // init V to be only 0's
    for (int i = 0; i<NODE_COUNT+1; i++){
        V[i] = 0;
    }

    // count how many edges each node has
    for (const auto& edge: EdgeList){
        V[edge.first]++;
    }

    // prefix sums for V
    for (int i = 1; i<NODE_COUNT+1; i++){
        V[i]+=V[i-1];
    }

    // init E
    for (const auto& edge: EdgeList){
        E[--V[edge.first]] = edge.second;
    }

    return std::make_pair(V, E);
}

// read a Graph File
// TODO: change NODE_COUNT, EDGE_COUNT system
void readGraphFile (std::string filename){
    std::string line;
    std::ifstream MyReadFile (filename);

    // Read Number of Nodes (n) and Number of Edges (m)
    getline (MyReadFile, line);
    std::istringstream iss(line);
    int n, m;
    iss >> n;
    iss >> m;
    std::cout << n << " " << m << "\n";

    // Read edges
    std::vector<std::pair<int, int>> EdgeList;
    while (getline (MyReadFile, line)){
        std::istringstream iss(line);
        int u, v;
        iss >> u;
        std::cout << u << "\n";
        u--;
        do {
            iss >> v;
            v--;
            EdgeList.push_back(std::make_pair(u, v));
        } while (iss);
    }

    std::pair<std::array<int, NODE_COUNT+1>, std::array<int, EDGE_COUNT>> graph = adjacencyArray(EdgeList);
    //printGraphShort(graph, 10);
}



int main(int argc, char** argv) {
    // Start timer
    auto start = std::chrono::high_resolution_clock::now();

    if (argc < 5){
        std::cout << "Usage: Source Node, Target Node, Graph Filename, Coordinates Filename\n";
        return 0;
    }

    // Reading Arguments from Command Line
    int s = std::stoi(argv[1]);
    int t = std::stoi(argv[2]);
    std::string gfile = argv[3];
    std::string cfile = argv[4];

    readGraphFile(gfile);


    /*
    // Example Edgelist
    std::vector<std::pair<int, int>> EdgeList;
    EdgeList.push_back(std::make_pair(0, 1));
    EdgeList.push_back(std::make_pair(0, 2));
    EdgeList.push_back(std::make_pair(1, 3));
    EdgeList.push_back(std::make_pair(1, 4));
    EdgeList.push_back(std::make_pair(2, 4));
    EdgeList.push_back(std::make_pair(4, 1));
    EdgeList.push_back(std::make_pair(4, 0));
    EdgeList.push_back(std::make_pair(4, 2));

    std::pair<std::array<int, NODE_COUNT+1>, std::array<int, EDGE_COUNT>> graph = adjacencyArray(EdgeList);
    printGraph(graph);  */

    // End Timer
    auto duration = std::chrono::high_resolution_clock::now() - start;
    long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
    std::cout << "Time taken by algorithm: " << microseconds <<  " microseconds.\n";

}
