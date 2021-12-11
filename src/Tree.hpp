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

    void FreeAllChildren(std::shared_ptr<Node>&n);

    void PrintAllChildren();

    void Luka();

    void Compress();

    void Dot(std::string name, bool withWords = true);

    void PrintLukaMap();

    void CompressionBDD();

    void FusionBDD(Tree &toFusionWith, std::string table = "");

    void UncompressionBDD();

    int CountNode();

   private:
    /****MEMBER VARIABLE****/
    std::shared_ptr<Node>_root;
    // Luka's words
    std::unordered_map<std::string, std::shared_ptr<Node>> _words;
    /****END-MEMBER VARIABLE****/

    void createTreeFromTable(std::shared_ptr<Node>&tree, int height, std::string &table);

    void printAllChildrenAux(std::shared_ptr<Node>&n, int stage = 0);

    void lukaAux(std::shared_ptr<Node>&n);

    void compressAux(std::shared_ptr<Node>&n);

    void defineInDot(std::shared_ptr<Node>&n, std::ofstream &f, bool withWords);

    void linkInDot(std::shared_ptr<Node>&n, std::ofstream &f,
                   std::unordered_set<std::shared_ptr<Node>> &marked);

    void compressionBDDAux(std::shared_ptr<Node>&n, std::shared_ptr<Node>&parent, int leftChild);

    void fusionBDDAux(std::shared_ptr<Node>&n, std::shared_ptr<Node>&toFusionWith, std::shared_ptr<Node>&fusionNode, std::string &table);

    void lukaFusionnedAux(std::shared_ptr<Node>&n);

    void uncompressionBDDAux(std::shared_ptr<Node>&n);

};
}  // namespace ex2ex3

#endif
