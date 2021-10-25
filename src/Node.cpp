#include "Node.h"

/*
Creating a new empty node
*/
std::shared_ptr<Node> newNode() {

  std::shared_ptr<Node> toReturn = std::make_shared<Node>();

  toReturn->_leftChild = nullptr;
  toReturn->_rightChild = nullptr;
  toReturn->_value = "";

  return toReturn;
}

/*
arg: 0 Initial Tree, 1 Tree to add
Insert a Tree in the Parents childrens
*/
std::shared_ptr<Node> insert(std::shared_ptr<Node> &initTree,
                             std::shared_ptr<Node> treeToAdd) {

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
