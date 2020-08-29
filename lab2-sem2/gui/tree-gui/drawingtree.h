#ifndef DRAWINGTREE_H
#define DRAWINGTREE_H

#include <algorithm>

enum drawingColor{White, Red, Black, Gray, Green};

struct drawingNode{
    int value;
    drawingNode* parent;
    drawingNode* left;
    drawingNode* right;
    drawingColor color;
    int x = 0, y = 0, radius = 0;

    drawingNode(int _value, drawingColor _color = White,
                drawingNode* _parent = nullptr,
                drawingNode* _left = nullptr,
                drawingNode* _right = nullptr){
        value = _value;
        color = _color;
        parent = _parent;
        left = _left;
        right = _right;
    }

    void addDrawingParamethers(int _x, int _y, int _r){
        x = _x;
        y = _y;
        radius = _r;
    }

    bool gotCoordinates(){return x * y != 0;}

};


class drawingTree
{
public:
    drawingTree();
    drawingTree(drawingNode* node);
    ~drawingTree();
    drawingNode* root = nullptr;
    int getHeight();

};

drawingNode* testTree1();

#endif // DRAWINGTREE_H
