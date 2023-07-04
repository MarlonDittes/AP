#ifndef HEAP_H
#define HEAP_H

#include <vector>
#include <unordered_map>
#include "node.h"

class BinaryHeap {
public:
    BinaryHeap();
    ~BinaryHeap();
    bool isEmpty() const;
    void insert(Node* node, double priority);
    void decreasePriority(Node* node, double priority);
    Node* deleteMin();
    void clear();

private:
    struct HeapNode {
        Node* node;
        double priority;
        int index;
    };
    std::vector<HeapNode> heap;

    int size;
    void heapifyUp(int index);
    void heapifyDown(int index);
};

#endif
