#ifndef __NODE_H__
#define __NODE_H__

#include <memory>
#include <string>

typedef struct node Node;
struct node {
    std::string _value;
    Node* _leftChild;
    Node* _rightChild;
};

Node* newNode(Node* left, Node* right, std::string value);
Node* insert(Node*& initTree, Node* treeToAdd);
int getTreeHeightFromWidth(int width);

#endif
