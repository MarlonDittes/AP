#ifndef NODE_H
#define NODE_H
#include <iostream>
#include <vector>

class Node{
    private:
        int index;
        double x,y;
        bool visited = false;
        bool boundary = false;
        int partition = -1;

    public:
        Node():
            index(0), x(0), y(0){};
        Node(int i, double x, double y):
            index(i), x(x), y(y){};

        int getIndex () { return this->index; };
        double getX () { return this->x; };
        double getY () { return this->y; };

        void setVisited (bool b) { this->visited = b; };
        bool isVisited () { return this->visited; };

        void setBoundary (bool b) { this->boundary = b; };
        bool isBoundary () { return this->boundary; };

        void setPartition (int p) { this->partition = p; };
        int getPartition () { return this->partition; };

        bool operator==(const Node& other) const {
            return index == other.index;
        }
};



#endif
