#include <iostream>
#include <string>
#include <vector>
#include <fstream>
    #include <sstream>

//Handling big int
#include <gmpxx.h>

//Question 1
std::string decomposition(const mpz_class & number);
std::string completion(std::string & number, int finalSize);
std::string table(const mpz_class & number, int n);

//Question2
typedef struct node Node;
struct node{
    std::string _value;
	Node*       _leftChild;
	Node*       _rightChild;
};

Node* insert(Node* initTree, Node* treeToAdd);
int getTreeHeightFromWidth(int width);
void createTreeFromTable(Node* tree, int height, std::string & table);
Node* consArbre(std::string table);
void freeAllChildren(Node* n);
void printAllChildren(int indentation, Node* n);
void luka(Node* n);
void defineInDot(Node* n, int height, std::ofstream & f);
void dot(Node* t);
void linkInDot(Node* n, std::ofstream & f);