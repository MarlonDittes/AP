#include <iostream>
#include <array>
#include <utility>
#include <vector>

const size_t NODE_COUNT = 5;
const size_t EDGE_COUNT = 8;

// print graph (AdjArr)
void printGraph (std::pair<std::array<int, NODE_COUNT+1>, std::array<int, EDGE_COUNT>> adjArr){
    std::cout << "Printing Graph: \n";

    // print V
    std::cout << "V: ";
    std::cout << adjArr.first[0];

    for (int i = 1; i<adjArr.first.size(); i++){
        std::cout << ", " << + adjArr.first[i];
    }
    std::cout << std::endl;

    // print E
    std::cout << "E: ";
    std::cout << adjArr.second[0];

    for (int i = 1; i<adjArr.second.size(); i++){
        std::cout << ", " << + adjArr.second[i];
    }
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



int main(int, char**) {
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
    printGraph(graph);


}
