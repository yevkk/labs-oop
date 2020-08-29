#include "drawingtree.h"

drawingTree::drawingTree()
{

}

drawingTree::drawingTree(drawingNode* node)
{
    root = node;
}

void deleteNode(drawingNode* cur){
    if(!cur)
        return;
    deleteNode(cur->left);
    deleteNode(cur->right);
    delete cur;
}

drawingTree::~drawingTree()
{
    deleteNode(root);
}

int getNodeHeight(drawingNode* node){
    if(!node)
        return 0;
    return std::max(getNodeHeight(node->left) + 1, getNodeHeight(node->right) + 1);
}

int drawingTree::getHeight(){
    return getNodeHeight(root);
}


drawingNode* testTree1(){
    drawingNode* node1 = new drawingNode(3,Black);
    drawingNode* node2 = new drawingNode(2,Black);
    drawingNode* node3 = new drawingNode(5,Red);
    drawingNode* node4 = new drawingNode(1,White);
    drawingNode* node5 = new drawingNode(4,White);
    drawingNode* node6 = new drawingNode(6,Gray);

    node1->left = node2;
    node1->right = node3;
    node2->left = node4;
    node3->left = node5;
    node3->right = node6;
    return node1;
}
