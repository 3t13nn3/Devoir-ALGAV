#ifndef __TREE_HPP__
#define __TREE_HPP__

#include <cmath>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "BTrie.hpp"
#include "Node.h"

enum CHILD { NOTHING, LEFT, RIGHT };

namespace ex2ex3 {
class Tree {
   public:
    Tree();

    Tree(std::shared_ptr<Node> &root);

    ~Tree();

    void ConsArbre(std::string &table);

    void PrintAllChildren();

    void Luka();

    void Compress();

    void Dot(std::string name, bool withWords = true);

    void PrintLukaBTrie();

    void CompressionBDD();

    int CountNode();

    Tree Meld(Tree &B, std::string table = "");

   private:
    /****MEMBER VARIABLE****/
    std::shared_ptr<Node> _root;
    // Luka's words
    BTrie *_words;
    /****END-MEMBER VARIABLE****/

    void createTreeFromTable(std::shared_ptr<Node> &tree, int height,
                             std::string &table);

    void printAllChildrenAux(std::shared_ptr<Node> &n, int stage = 0);

    void lukaAux(std::shared_ptr<Node> &n);

    void compressAux(std::shared_ptr<Node> &n);

    void defineInDot(std::shared_ptr<Node> &n, std::ofstream &f,
                     bool withWords);

    void linkInDot(std::shared_ptr<Node> &n, std::ofstream &f,
                   std::unordered_set<std::shared_ptr<Node>> &marked);

    void compressionBDDAux(std::shared_ptr<Node> &n,
                           std::shared_ptr<Node> &parent, int leftChild);

    void fusionBDDAux(std::shared_ptr<Node> &n,
                      std::shared_ptr<Node> &toFusionWith,
                      std::shared_ptr<Node> &fusionNode, std::string &table);

    void lukaFusionnedAux(std::shared_ptr<Node> &n);

    void countNodeAux(std::shared_ptr<Node> &,
                      std::unordered_set<std::shared_ptr<Node>> &marked);

    std::shared_ptr<Node> meldAux(std::shared_ptr<Node> &A,
                                  std::shared_ptr<Node> &B, std::string table);
};
}  // namespace ex2ex3

#endif
