#include "heap.h"
#include "node.h"
#include <unordered_map>

std::size_t NodeHash::operator()(const Node& node) const {
    return std::hash<int>()(node.getIndex());
};

bool BinaryHeap::isEmpty() const {
    return heap.empty();
}

void BinaryHeap::insert(Node node, double priority) {
    if (indices.count(node) > 0) {
        throw std::invalid_argument("Node already exists in heap");
    }
    heap.emplace_back(node, priority);
    indices[node] = heap.size() - 1;
    heapifyUp(heap.size() - 1);
}

void BinaryHeap::remove(Node node) {
    auto it = indices.find(node);
    if (it == indices.end()) {
        return;
    }
    int index = it->second;
    swapNodes(index, heap.size() - 1);
    heap.pop_back();
    indices.erase(it);
    heapifyDown(index);
}

void BinaryHeap::decreasePriority(Node node, double priority) {
    auto it = indices.find(node);
    if (it == indices.end()) {
        return;
    }
    int index = it->second;
    if (priority < heap[index].priority) {
        heap[index].priority = priority;
        heapifyUp(index);
    }
}

Node BinaryHeap::top() const {
    if (isEmpty()) {
        throw std::out_of_range("Heap is empty");
    }
    return heap[0].node;
}

void BinaryHeap::pop() {
    if (isEmpty()) {
        return;
    }
    const Node& node = heap[0].node;
    indices.erase(node);
    swapNodes(0, heap.size() - 1);
    heap.pop_back();
    heapifyDown(0);
}

void BinaryHeap::heapifyUp(int index) {
    int parentIndex = (index - 1) / 2;
    while (index > 0 && heap[index].priority < heap[parentIndex].priority) {
        swapNodes(index, parentIndex);
        index = parentIndex;
        parentIndex = (index - 1) / 2;
    }
}

void BinaryHeap::heapifyDown(int index) {
    int leftChildIndex = 2 * index + 1;
    int rightChildIndex = 2 * index + 2;
    int minIndex = index;
    if (leftChildIndex < heap.size() && heap[leftChildIndex].priority < heap[minIndex].priority) {
        minIndex = leftChildIndex;
    }
    if (rightChildIndex < heap.size() && heap[rightChildIndex].priority < heap[minIndex].priority) {
        minIndex = rightChildIndex;
    }
    if (minIndex != index) {
        swapNodes(index, minIndex);
        heapifyDown(minIndex);
    }
}

void BinaryHeap::swapNodes(int index1, int index2) {
    std::swap(heap[index1], heap[index2]);
    indices[heap[index1].node] = index1;
    indices[heap[index2].node] = index2;
}
