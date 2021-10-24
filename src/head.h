#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// Handling big int
#include <gmpxx.h>

// Question 1
std::string decomposition(const mpz_class &number);
std::string completion(std::string &number, int finalSize);
std::string table(const mpz_class &number, int n);

// Question 2
typedef struct node Node;
struct node {
  std::string _value;
  std::shared_ptr<Node> _leftChild;
  std::shared_ptr<Node> _rightChild;
};

std::shared_ptr<Node> newNode();
std::shared_ptr<Node> insert(std::shared_ptr<Node> &initTree,
                             std::shared_ptr<Node> treeToAdd);
int getTreeHeightFromWidth(int width);
void createTreeFromTable(std::shared_ptr<Node> &tree, int height,
                         std::string &table);
std::shared_ptr<Node> consArbre(std::string &table);
// void freeAllChildren(std::shared_ptr<Node>&n);
void printAllChildren(int indentation, std::shared_ptr<Node> &n);
void luka(std::shared_ptr<Node> &n);
void defineInDot(std::shared_ptr<Node> &n, int height, std::ofstream &f,
                 bool withWords);
void dot(std::shared_ptr<Node> &t, bool withWords);
void linkInDot(std::shared_ptr<Node> &n, std::ofstream &f,
               std::unordered_set<std::shared_ptr<Node>> &marked);
void compress(std::shared_ptr<Node> &n);

// Question 3
void compressionBDD(std::shared_ptr<Node> n);