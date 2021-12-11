#include "Tree.hpp"

using namespace ex2ex3;

Tree::Tree() { _root = newNode(nullptr, nullptr, ""); }

Tree::~Tree() { FreeAllChildren(_root); }

/*
arg: 0 Initial Tree, 1 Height of the tree, 2 table of truth
Create a Tree of a Height "height", based on DFS, putting the table of truth
value at leafs
*/
void Tree::createTreeFromTable(std::shared_ptr<Node> &tree, int height,
                               std::string &table) {
    if (height == 0) {
        tree->_value = table[0];

        // removing the first element from table
        table.erase(0, 1);
        return;
    }

    insert(tree, newNode(nullptr, nullptr, ""));
    createTreeFromTable(tree->_leftChild, height - 1, table);

    insert(tree, newNode(nullptr, nullptr, ""));
    createTreeFromTable(tree->_rightChild, height - 1, table);
}

/*
arg: 0 table of truth
Create a Tree of a certain height in function of the table of truth
*/
void Tree::ConsArbre(std::string &table) {
    createTreeFromTable(_root, getTreeHeightFromWidth(table.size()), table);
}

/*
arg: 0 Root node
Free all the children of a given node
*/
void Tree::FreeAllChildren(std::shared_ptr<Node> &n) {
    if (n == nullptr) {
        return;
    }

    FreeAllChildren(n->_leftChild);

    FreeAllChildren(n->_rightChild);

    // n = nullptr;
    // delete n;
    // n = nullptr;
}

/*
arg: 0 the root, 1 stage of the node (call it with 0, a root is always as 0)
Print all the children of a given node
*/
void Tree::printAllChildrenAux(std::shared_ptr<Node> &n, int stage) {
    if (n == nullptr) {
        return;
    }

    printAllChildrenAux(n->_leftChild, stage + 1);

    printAllChildrenAux(n->_rightChild, stage + 1);

    int i;
    for (i = 0; i < stage; i++) {
        std::cout << "- - - -\t\t";
    }
    std::cout << "[" << stage << "]"
              << "Node: ";
    std::cout << n->_value << " <> " << n << std::endl;
}

// Calling aux function to avoid problemes on recursion with the reference
// of a class member
void Tree::PrintAllChildren() { printAllChildrenAux(_root); }

/*
arg: 0 Node to applied luka function (the root generaly)
Assign a Luka's word for each node and fill our _words map with the nodes we
browse. Based on BFS.
*/
void Tree::lukaAux(std::shared_ptr<Node> &n) {
    // If we are on leafs
    if (n->_leftChild == nullptr) {
        if (_words.find(n->_value) == _words.end()) {
            // not found
            _words[n->_value] = n;
            // std::cout << n->_value << std::endl;
        }

        return;
    }

    lukaAux(n->_leftChild);

    lukaAux(n->_rightChild);

    n->_value = n->_leftChild->_value + n->_rightChild->_value;

    // adding word to our map
    //_words[n->_value] = n;
    if (_words.find(n->_value) == _words.end()) {
        // not found
        _words[n->_value] = n;
        // std::cout << n->_value << std::endl;
    }
}

// Calling aux function to avoid problemes on recursion with the reference
// of a class member
void Tree::Luka() {
    if (_root->_value.find("|") != std::string::npos) {
        lukaFusionnedAux(_root);
    } else {
        lukaAux(_root);
    }
}

/*
arg: 0 Node to applied compress function (the root generaly)
Check for each node the associate node of the current Luka's word in our map
(_words). If nodes aren't the same, we assign the node in the map to the
current node. Based on BFS.
*/
void Tree::compressAux(std::shared_ptr<Node> &n) {
    if (n == nullptr) {
        return;
    }

    compressAux(n->_leftChild);

    compressAux(n->_rightChild);

    if (n != _words[n->_value]) {
        auto tmp = n;
        n = _words[n->_value];
        // delete tmp;
    }
}

// Calling aux function to avoid problemes on recursion with the reference
// of a class member
void Tree::Compress() {
    compressAux(_root);
    _root = _words[_root->_value];
}

/*
arg: 0 Initial Node, 1 Height of the tree, 2 stream to the file we are
writting, 3 boolean to indicate if we want to put luka's word as label
Define all nodes in our dot file with the adresse of each node. We are
labelling them with the current height of a node.
*/
void Tree::defineInDot(std::shared_ptr<Node> &n, std::ofstream &f,
                       bool withWords) {
    if (n == nullptr) {
        return;
    }

    const void *address = static_cast<const void *>(n.get());
    std::stringstream ss;
    ss << address;
    std::string name = ss.str();
    name = "_" + name + "_";

    int height = 0;
    if (n->_value.find("|") != std::string::npos) {
        std::string delimiter = ">=";
        std::string token = n->_value.substr(0, n->_value.find(" | "));
        height = getTreeHeightFromWidth(token.length());
    } else {
        height = getTreeHeightFromWidth(n->_value.length());
    }

    std::string toWrite = "";
    if (withWords) {
        if (n->_leftChild == nullptr && n->_rightChild == nullptr) {
            toWrite = name + "[label=\"[" + n->_value + "]\"];";
        } else {
            toWrite = name + "[label=\"x" + std::to_string(height) + " [" +
                      n->_value + "]\"];";
        }
    } else {
        if (n->_leftChild == nullptr && n->_rightChild == nullptr) {
            toWrite = name + "[label=\"[" + n->_value + "]\"];";
        } else {
            toWrite = name + "[label=\"x" + std::to_string(height) + "\"];";
        }
    }

    f << toWrite;
    defineInDot(n->_leftChild, f, withWords);

    defineInDot(n->_rightChild, f, withWords);
}

/*
arg: 0 Initial Node, 1 stream to the file we are writting, 2 a set of nodes
we have already process Link every nodes to theire child in the dot file.
Based on nodes adresses. We need to check if we have already process a node
to avoid multiple link.
*/
void Tree::linkInDot(std::shared_ptr<Node> &n, std::ofstream &f,
                     std::unordered_set<std::shared_ptr<Node>> &marked) {
    if (n == nullptr) {
        return;
    }

    if ((marked.find(n) != marked.end()) == false) {  // if not in the set

        marked.insert(n);

        const void *address = static_cast<const void *>(n.get());
        std::stringstream ss;
        ss << address;
        std::string father = ss.str();
        father = "_" + father + "_";

        if (n->_leftChild != nullptr) {
            const void *address =
                static_cast<const void *>(n->_leftChild.get());
            std::stringstream ss;
            ss << address;
            std::string child = ss.str();
            child = "_" + child + "_";

            std::string toWrite = father + "->" + child + "[style=dashed];";
            f << toWrite;
        }

        if (n->_rightChild != nullptr) {
            const void *address =
                static_cast<const void *>(n->_rightChild.get());
            std::stringstream ss;
            ss << address;
            std::string child = ss.str();
            child = "_" + child + "_";

            std::string toWrite = father + "->" + child + ";";
            f << toWrite;
        }

        linkInDot(n->_leftChild, f, marked);

        linkInDot(n->_rightChild, f, marked);
    }
}

/*
arg: 0 boolean to indicate if we want to put luka's word as label
Fill the dot file by calling defineInDot() and linkInDot().
*/
void Tree::Dot(std::string name, bool withWords) {
    // exemple:
    // digraph{a->bb[style=dashed];b[label=\"som1\"];b->c[labal="som1"];a->c;d->c;e->c;e->a;}

    system("mkdir ../tree");
    std::ofstream dotFile("../tree/" + name + ".dot");

    std::unordered_set<std::shared_ptr<Node>> marked;

    if (dotFile.is_open()) {
        dotFile << "digraph{";

        defineInDot(_root, dotFile, withWords);

        linkInDot(_root, dotFile, marked);

        dotFile << "}";

        dotFile.close();
    }

    std::cout << "using dot -Tsvg -o ../tree/" << name << ".svg ../tree/"
              << name << ".dot" << std::endl;

    // creating tree image
    system(("dot -Tsvg -o ../tree/" + name + ".svg ../tree/" + name + ".dot")
               .c_str());
}

/*
Print the content of luka's map
*/
void Tree::PrintLukaMap() {
    std::cout << "Luka's Map:" << std::endl;
    for (const auto &e : _words) {
        std::cout << e.first << std::endl;
    }
}

/*
arg: 0 Node to applied compress function (the root of the BDD generaly), 1
Father node of the arg 0, 2 flag that can tell us where we have recovery the
node from Check for each childs of the current node the associate node of
the current Luka's word in our map (_words). If nodes aren't the same, we
assign the node in the map to the current node. After that, we need to apply
the deletion rule that say if both childs of a node have the same value
(then are the same because we replaced them by the associate map value), so
we could remove it ant link there father to there child Based on BFS.
*/
void Tree::compressionBDDAux(std::shared_ptr<Node> &n,
                             std::shared_ptr<Node> &parent, int from) {
    if (n == nullptr) {
        return;
    }

    compressionBDDAux(n->_leftChild, n, LEFT);
    compressionBDDAux(n->_rightChild, n, RIGHT);

    // std::cout << n->_value << std::endl;
    //  then the deletion rule
    if (n->_leftChild != nullptr && n->_rightChild != nullptr &&
        n->_leftChild->_value == n->_rightChild->_value) {
        // update lukas map
        _words.erase(n->_value);
        auto cpy = n;
        if (from == NOTHING) {
            n = n->_leftChild;
        } else if (from == LEFT) {
            parent->_leftChild = n->_leftChild;
        } else if (from == RIGHT) {
            parent->_rightChild = n->_rightChild;
        }
        // delete cpy;
    }

    // compress init
    if (n != _words[n->_value]) {
        auto tmp = n;
        n = _words[n->_value];
        // delete tmp;
    }
}

// Calling aux function to avoid problemes on recursion with the reference
// of a class member
void Tree::CompressionBDD() { compressionBDDAux(_root, _root, NOTHING); }

void Tree::fusionBDDAux(std::shared_ptr<Node> &n,
                        std::shared_ptr<Node> &toFusionWith,
                        std::shared_ptr<Node> &fusionNode, std::string &table) {
    if (n == nullptr) {
        return;
    }

    /*RECONSTRUCT MISSING NODES*/
    if (n->_leftChild != nullptr) {
        auto current = n;
        while (current->_leftChild->_value.length() <
               current->_value.length() / 2) {
            auto tmp = current->_leftChild;
            current->_leftChild = newNode(tmp, tmp, tmp->_value + tmp->_value);
        }
    }
    if (n->_rightChild != nullptr) {
        auto current = n;
        while (current->_rightChild->_value.length() <
               current->_value.length() / 2) {
            auto tmp = current->_rightChild;
            current->_rightChild = newNode(tmp, tmp, tmp->_value + tmp->_value);
        }
    }

    if (toFusionWith->_leftChild != nullptr) {
        auto current = toFusionWith;
        while (current->_leftChild->_value.length() <
               current->_value.length() / 2) {
            auto tmp = current->_leftChild;
            current->_leftChild = newNode(tmp, tmp, tmp->_value + tmp->_value);
        }
    }
    if (toFusionWith->_rightChild != nullptr) {
        auto current = toFusionWith;
        while (current->_rightChild->_value.length() <
               current->_value.length() / 2) {
            auto tmp = current->_rightChild;
            current->_rightChild = newNode(tmp, tmp, tmp->_value + tmp->_value);
        }
    }
    /*RECONSTRUCT MISSING NODES -- END*/

    if (n->_value.size() == toFusionWith->_value.size()) {
        fusionNode->_value = n->_value + " | " + toFusionWith->_value;
    } else if (n->_value.size() < toFusionWith->_value.size()) {
        n->_value += n->_value;
        fusionNode->_value = n->_value + " | " + toFusionWith->_value;
    } else {
        toFusionWith->_value += toFusionWith->_value;
        fusionNode->_value = n->_value + " | " + toFusionWith->_value;
    }

    // process with the table of truth
    //"0-0,0-1,1-0,1-1"
    if (table != "") {
        std::vector<std::string> values;
        size_t pos = 0;

        while ((pos = fusionNode->_value.find(" | ")) != std::string::npos) {
            std::string token = "";
            token = fusionNode->_value.substr(0, pos);

            values.emplace_back(token);
            fusionNode->_value.erase(0, pos + std::string(" | ").length());
        }
        values.emplace_back(fusionNode->_value);

        fusionNode->_value = "";
        for (size_t i(0); i < values[0].length(); ++i) {
            if (values[0] == "0" && values[1] == "0") {
                fusionNode->_value += table[0];
            } else if (values[0] == "0" && values[1] == "1") {
                fusionNode->_value += table[1];
            } else if (values[0] == "1" && values[1] == "0") {
                fusionNode->_value += table[2];
            } else if (values[0] == "1" && values[1] == "1") {
                fusionNode->_value += table[3];
            }
        }
    }

    if (n->_leftChild != nullptr || toFusionWith->_leftChild != nullptr) {
        fusionNode->_leftChild = newNode(nullptr, nullptr, "");
        if (n->_leftChild == nullptr) {
            fusionBDDAux(n, toFusionWith->_leftChild, fusionNode->_leftChild,
                         table);
        } else if (toFusionWith->_leftChild == nullptr) {
            fusionBDDAux(n->_leftChild, toFusionWith, fusionNode->_leftChild,
                         table);
        } else {
            fusionBDDAux(n->_leftChild, toFusionWith->_leftChild,
                         fusionNode->_leftChild, table);
        }
    }
    if (n->_rightChild != nullptr || toFusionWith->_rightChild != nullptr) {
        fusionNode->_rightChild = newNode(nullptr, nullptr, "");
        if (n->_rightChild == nullptr) {
            fusionBDDAux(n, toFusionWith->_rightChild, fusionNode->_rightChild,
                         table);
        } else if (toFusionWith->_rightChild == nullptr) {
            fusionBDDAux(n->_rightChild, toFusionWith, fusionNode->_rightChild,
                         table);
        } else {
            fusionBDDAux(n->_rightChild, toFusionWith->_rightChild,
                         fusionNode->_rightChild, table);
        }
    }
}
//"0-0,0-1,1-0,1-1"
void Tree::FusionBDD(Tree &toFusionWith, std::string table) {
    /*uncompressing our trees because we need deletation rules off for an easier
     * work tree*/
    // this->UncompressionBDD();
    // toFusionWith.UncompressionBDD();

    std::shared_ptr<Node> fusionNode = newNode(nullptr, nullptr, "");
    _words.clear();
    fusionBDDAux(_root, toFusionWith._root, fusionNode, table);
    _root = fusionNode;
}

void Tree::lukaFusionnedAux(std::shared_ptr<Node> &n) {
    // If we are on leafs
    if (n->_leftChild == nullptr) {
        if (_words.find(n->_value) == _words.end()) {
            // not found
            _words[n->_value] = n;
        }
        return;
    }

    if (_words.find(n->_value) == _words.end()) {
        // not found
        _words[n->_value] = n;
        // std::cout << n->_value << std::endl;
    }

    lukaFusionnedAux(n->_leftChild);

    lukaFusionnedAux(n->_rightChild);
}

void Tree::uncompressionBDDAux(std::shared_ptr<Node> &n) {
    if (n == nullptr) {
        return;
    }

    if (n->_leftChild != nullptr) {
        auto current = n;
        while (current->_leftChild->_value.length() <
               current->_value.length() / 2) {
            auto tmp = current->_leftChild;
            current->_leftChild = newNode(tmp, tmp, tmp->_value + tmp->_value);
        }
    }
    if (n->_rightChild != nullptr) {
        auto current = n;
        while (current->_rightChild->_value.length() <
               current->_value.length() / 2) {
            auto tmp = current->_rightChild;
            current->_rightChild = newNode(tmp, tmp, tmp->_value + tmp->_value);
        }
    }
    uncompressionBDDAux(n->_leftChild);
    uncompressionBDDAux(n->_rightChild);
}

void Tree::UncompressionBDD() { uncompressionBDDAux(_root); }

int Tree::CountNode() {
    int n = 0;

    for (const auto &_ : _words) {
        std::ignore = _;
        ++n;
    }

    return n;
}
