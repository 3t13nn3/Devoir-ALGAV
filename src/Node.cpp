#include "Node.h"

/*
Creating a new empty node
*/
Node* newNode(Node* left, Node* right, std::string value) {
    Node* toReturn = (Node*)malloc(sizeof(Node));

    toReturn->_leftChild = left;
    toReturn->_rightChild = right;
    toReturn->_value = value;

    return toReturn;
}

/*
arg: 0 Initial Tree, 1 Tree to add
Insert a Tree in the Parents childrens
*/
Node* insert(Node*& initTree, Node* treeToAdd) {
    if (initTree->_leftChild == nullptr) {
        initTree->_leftChild = treeToAdd;
    } else {
        initTree->_rightChild = treeToAdd;
    }

    return treeToAdd;
}

/*
arg: 0 width of the tree (length of the table of truth generaly)
return the height of the tree
*/
int getTreeHeightFromWidth(int width) {
    int power = 1;

    // power is the size of our tree (pow of 2)
    while (1 << power < width) {
        power++;
    }

    return power;
}
