#ifndef __NODE_H__
#define __NODE_H__

#include <memory>
#include <string>

typedef struct node Node;
struct node {
    std::string _value;
    std::shared_ptr<Node> _leftChild;
    std::shared_ptr<Node> _rightChild;
};

std::shared_ptr<Node> newNode(std::shared_ptr<Node> left, std::shared_ptr<Node> right, std::string value);
std::shared_ptr<Node> insert(std::shared_ptr<Node>& initTree, std::shared_ptr<Node> treeToAdd);
int getTreeHeightFromWidth(int width);

#endif
