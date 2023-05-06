#ifndef HEAP_H
#define HEAP_H

#include <vector>
#include <unordered_map>
#include "node.h"

class BinaryHeap {
public:
    BinaryHeap();
    bool isEmpty() const;
    void insert(Node node, double priority);
    void remove(Node node);
    void decreasePriority(Node node, double priority);
    Node top() const;
    void pop();

private:
    struct HeapNode {
        Node node;
        double priority;
    };
    std::vector<HeapNode> heap;
    std::unordered_map<Node, int, NodeHash> indices;

    void heapifyUp(int index);
    void heapifyDown(int index);
    void swapNodes(int index1, int index2);
};

struct NodeHash {
    size_t operator()(const Node& node) const;
};

#endif
