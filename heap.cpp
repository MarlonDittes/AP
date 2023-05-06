#include "heap.h"
#include "node.h"
#include <unordered_map>

BinaryHeap::BinaryHeap(){
    this->size = 0;
}

BinaryHeap::~BinaryHeap() {
    this->clear();
}

bool BinaryHeap::isEmpty() const {
    return size == 0;
}

void BinaryHeap::insert(Node* node, double priority) {
    HeapNode heapNode;
    heapNode.node = node;
    heapNode.priority = priority;
    heapNode.index = size;

    heap.push_back(heapNode);
    indices[node] = size;
    size++;
    heapifyUp(heapNode.index);
}

void BinaryHeap::decreasePriority(Node* node, double priority) {
    int index = indices[node];
    heap[index].priority = priority;
    heapifyUp(index);
}

Node* BinaryHeap::deleteMin() {
    if (isEmpty()) {
        return nullptr;
    }

    Node *minNode = heap[0].node;
    indices.erase(minNode);

    size--;
    if (size > 0) {
        HeapNode lastNode = heap[size];
        heap[0] = lastNode;
        indices[lastNode.node] = 0;
        heapifyDown(0);
    }

    heap.pop_back();

    return minNode;
}

void BinaryHeap::clear() {
    for (auto heapNode : heap) {
        delete heapNode.node;
    }
    heap.clear();
    indices.clear();
    size = 0;
}

void BinaryHeap::heapifyUp(int index) {
    HeapNode heapNode = heap[index];
    while (index > 0) {
        int parentIndex = (index - 1) / 2;
        HeapNode parent = heap[parentIndex];
        if (heapNode.priority < parent.priority) {
            heap[index] = parent;
            indices[parent.node] = index;
            index = parentIndex;
        } else {
            break;
        }
    }
    heap[index] = heapNode;
    indices[heapNode.node] = index;
}

void BinaryHeap::heapifyDown(int index) {
    HeapNode heapNode = heap[index];
    int leftChildIndex = 2 * index + 1;
    int rightChildIndex = 2 * index + 2;
    int minIndex = index;

    if (leftChildIndex < size) {
        HeapNode leftChild = heap[leftChildIndex];
        if (leftChild.priority < heapNode.priority) {
            minIndex = leftChildIndex;
        }
    }

    if (rightChildIndex < size) {
        HeapNode rightChild = heap[rightChildIndex];
        if (rightChild.priority < heapNode.priority && rightChild.priority < heap[minIndex].priority) {
            minIndex = rightChildIndex;
        }
    }

    if (minIndex != index) {
        HeapNode minNode = heap[minIndex];
        heap[index] = minNode;
        indices[minNode.node] = index;
        heap[minIndex] = heapNode;
        indices[heapNode.node] = minIndex;
        heapifyDown(minIndex);
    }
}

void BinaryHeap::swapNodes(int index1, int index2) {
    HeapNode node1 = heap[index1];
    HeapNode node2 = heap[index2];
    heap[index1] = node2;
    heap[index2] = node1;
    indices[node1.node] = index2;
    indices[node2.node] = index1;
}
