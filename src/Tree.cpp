#include "Tree.hpp"

using namespace ex2ex3;

Tree::Tree() { _root = newNode(); }

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

    insert(tree, newNode());
    createTreeFromTable(tree->_leftChild, height - 1, table);

    insert(tree, newNode());
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
/*
void freeAllChildren(std::shared_ptr<Node> &n) {

  if (n.get() == nullptr) {
    return;
  }

  freeAllChildren(n->_leftChild);

  freeAllChildren(n->_rightChild);

  n = nullptr;

  if (n == nullptr) { // needed because a same node could be deleted and we
                      // don't want this
    //delete n;
  }
}
*/

/*
arg: 0 the root, 1 stage of the node (call it with 0, a root is always as 0)
Print all the children of a given node
*/
void Tree::printAllChildrenAux(std::shared_ptr<Node> &n, int stage) {
    if (n.get() == nullptr) {
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
    std::cout << n->_value << std::endl;
}

// Calling aux function to avoid problemes on recursion with the reference of a
// class member
void Tree::PrintAllChildren() { printAllChildrenAux(_root); }

/*
arg: 0 Node to applied luka function (the root generaly)
Assign a Luka's word for each node and fill our _words map with the nodes we
browse. Based on BFS.
*/
void Tree::lukaAux(std::shared_ptr<Node> &n) {
    // If we are on leafs
    if (n->_leftChild.get() == nullptr) {
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

// Calling aux function to avoid problemes on recursion with the reference of a
// class member
void Tree::Luka() { lukaAux(_root); }

/*
arg: 0 Node to applied compress function (the root generaly)
Check for each node the associate node of the current Luka's word in our map
(_words). If nodes aren't the same, we assign the node in the map to the current
node. Based on BFS.
*/
void Tree::compressAux(std::shared_ptr<Node> &n) {
    if (n.get() == nullptr) {
        return;
    }

    compressAux(n->_leftChild);

    compressAux(n->_rightChild);

    if (n != _words[n->_value]) {
        // freeAllChildren(n->_leftChild);
        // freeAllChildren(n->_rightChild);

        n = _words[n->_value];
    }
}

// Calling aux function to avoid problemes on recursion with the reference of a
// class member
void Tree::Compress() { compressAux(_root); }

/*
arg: 0 Initial Node, 1 Height of the tree, 2 stream to the file we are writting,
3 boolean to indicate if we want to put luka's word as label
Define all nodes in our dot file with the adresse of each node. We are labelling
them with the current height of a node.
*/
void Tree::defineInDot(std::shared_ptr<Node> &n, int height, std::ofstream &f,
                       bool withWords) {
    if (n.get() == nullptr) {
        return;
    }

    const void *address = static_cast<const void *>(n.get());
    std::stringstream ss;
    ss << address;
    std::string name = ss.str();
    name = "_" + name + "_";

    std::string toWrite = "";
    if (withWords) {
        toWrite = name + "[label=\"x" + std::to_string(height) + " [" + n->_value +
                  "]\"];";
    } else {
        if (n->_leftChild.get() == nullptr && n->_rightChild.get() == nullptr) {
            toWrite = name + "[label=\"x" + std::to_string(height) + " [" +
                      n->_value + "]\"];";
        } else {
            toWrite = name + "[label=\"x" + std::to_string(height) + "\"];";
        }
    }

    f << toWrite;
    defineInDot(n->_leftChild, height - 1, f, withWords);

    defineInDot(n->_rightChild, height - 1, f, withWords);
}

/*
arg: 0 Initial Node, 1 stream to the file we are writting, 2 a set of nodes we
have already process
Link every nodes to theire child in the dot file. Based on
nodes adresses. We need to check if we have already process a node to avoid
multiple link.
*/
void Tree::linkInDot(std::shared_ptr<Node> &n, std::ofstream &f,
                     std::unordered_set<std::shared_ptr<Node>> &marked) {
    if (n.get() == nullptr) {
        return;
    }

    if ((marked.find(n) != marked.end()) == false) {  // if not in the set

        marked.insert(n);

        const void *address = static_cast<const void *>(n.get());
        std::stringstream ss;
        ss << address;
        std::string father = ss.str();
        father = "_" + father + "_";

        if (n->_leftChild.get() != nullptr) {
            const void *address = static_cast<const void *>(n->_leftChild.get());
            std::stringstream ss;
            ss << address;
            std::string child = ss.str();
            child = "_" + child + "_";

            std::string toWrite = father + "->" + child + "[style=dashed];";
            f << toWrite;
        }

        if (n->_rightChild.get() != nullptr) {
            const void *address = static_cast<const void *>(n->_rightChild.get());
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
void Tree::Dot(bool withWords) {
    // exemple:
    // digraph{a->bb[style=dashed];b[label=\"som1\"];b->c[labal="som1"];a->c;d->c;e->c;e->a;}

    system("mkdir ../tree");
    std::ofstream dotFile("../tree/tree.dot");

    std::unordered_set<std::shared_ptr<Node>> marked;

    if (dotFile.is_open()) {
        dotFile << "digraph{";

        int height = getTreeHeightFromWidth(_root->_value.size());

        defineInDot(_root, height, dotFile, withWords);

        linkInDot(_root, dotFile, marked);

        dotFile << "}";

        dotFile.close();
    }

    std::cout << "using dot -Tsvg -o ../tree/tree.svg ../tree/tree.dot"
              << std::endl;

    // creating tree image
    system("dot -Tsvg -o ../tree/tree.svg ../tree/tree.dot");
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

void Tree::compressionBDDAux(std::shared_ptr<Node> &n, std::shared_ptr<Node> &parent, int childChooser) {
    if (n->_leftChild == nullptr) {
        return;
    }

    compressionBDDAux(n->_leftChild, n, LEFT);
    compressionBDDAux(n->_rightChild, n, RIGHT);

    // Deletion rule
    if (n->_leftChild->_value == n->_rightChild->_value) {
        std::cout << "Inside Deletion Rule" << std::endl;

        if (childChooser == -1) {
            n = n->_leftChild;
        } else if (childChooser == LEFT) {
            parent->_leftChild = n->_leftChild;
        } else {  // RIGHT
            parent->_rightChild = n->_leftChild;
        }
    }
}

void Tree::CompressionBDD() { compressionBDDAux(_root, _root, -1); }
