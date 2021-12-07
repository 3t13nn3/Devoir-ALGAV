#ifndef __TREE_HPP__
#define __TREE_HPP__

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <cmath>
#include <vector>

#include "Node.h"

enum CHILD { NOTHING, LEFT, RIGHT };

namespace ex2ex3 {
class Tree {
   public:
    Tree();

    ~Tree();

    void ConsArbre(std::string &table);

    void FreeAllChildren(Node *&n);

    void PrintAllChildren();

    void Luka();

    void Compress();

    void Dot(std::string name, bool withWords = true);

    void PrintLukaMap();

    void CompressionBDD();

    void FusionBDD(Tree &toFusionWith, std::string table = "");

    void UncompressionBDD();

   private:
    /****MEMBER VARIABLE****/
    Node *_root;
    // Luka's words
    std::unordered_map<std::string, Node *> _words;
    /****END-MEMBER VARIABLE****/

    void createTreeFromTable(Node *&tree, int height, std::string &table);

    void printAllChildrenAux(Node *&n, int stage = 0);

    void lukaAux(Node *&n);

    void compressAux(Node *&n);

    void defineInDot(Node *&n, int height, std::ofstream &f, bool withWords);

    void linkInDot(Node *&n, std::ofstream &f,
                   std::unordered_set<Node *> &marked);

    void compressionBDDAux(Node *&n, Node *&parent, int leftChild);

    void fusionBDDAux(Node *&n, Node *&toFusionWith, Node *&fusionNode, std::string table);

    void lukaFusionnedAux(Node *&n);

    void uncompressionBDDAux(Node *&n);

};
}  // namespace ex2ex3

#endif
