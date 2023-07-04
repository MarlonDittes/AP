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
    node->setHeapIndex(size);

    heap.push_back(heapNode);
    size++;
    heapifyUp(node->getHeapIndex());
}

void BinaryHeap::decreasePriority(Node* node, double priority) {
    int index = node->getHeapIndex();
    heap[index].priority = priority;
    heapifyUp(index);
}

Node* BinaryHeap::deleteMin() {
    if (isEmpty()) {
        return nullptr;
    }

    Node *minNode = heap[0].node;

    size--;
    if (size > 0) {
        HeapNode lastNode = heap[size];
        heap[0] = lastNode;
        lastNode.node->setHeapIndex(0);
        heapifyDown(0);
    }

    heap.pop_back();

    return minNode;
}

void BinaryHeap::clear() {
    /*for (auto heapNode : heap) {
        delete heapNode.node;
    }*/
    heap.clear();
    size = 0;
}

void BinaryHeap::heapifyUp(int index) {
    HeapNode heapNode = heap[index];
    while (index > 0) {
        int parentIndex = (index - 1) / 2;
        HeapNode parent = heap[parentIndex];
        if (heapNode.priority < parent.priority) {
            heap[index] = parent;
            parent.node->setHeapIndex(index);
            index = parentIndex;
        } else {
            break;
        }
    }
    heap[index] = heapNode;
    heapNode.node->setHeapIndex(index);
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
        minNode.node->setHeapIndex(index);
        heap[minIndex] = heapNode;
        heapNode.node->setHeapIndex(minIndex);
        heapifyDown(minIndex);
    }
}
