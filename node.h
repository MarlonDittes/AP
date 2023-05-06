#ifndef NODE_H
#define NODE_H
#include <iostream>
#include <vector>

class Node{
    private:
        int index;
        double x,y;
    public:
        Node(int i, double x, double y):
            index(i), x(x), y(y){};

        int getIndex () { return this->index; };
        double getX () { return this->x; };
        double getY () { return this->y; };
};

#endif
