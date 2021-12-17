#include "Tree.hpp"

using namespace ex2ex3;

Tree::Tree() {
    _root = newNode(nullptr, nullptr, "");
    _words = new BTrie();
}

Tree::~Tree() { _words->RemoveAll(); }

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
Assign a Luka's word for each node and fill our _words trie with the nodes we
browse. Based on BFS.
*/
void Tree::lukaAux(std::shared_ptr<Node> &n) {
    // If we are on leafs
    if (n->_leftChild == nullptr) {
        _words->CheckAndInsert(n);
        return;
    }

    lukaAux(n->_leftChild);

    lukaAux(n->_rightChild);

    n->_value = n->_leftChild->_value + n->_rightChild->_value;

    // adding word to our BTrie
    _words->CheckAndInsert(n);
}

// Calling aux function to avoid problemes on recursion with the reference
// of a class member
void Tree::Luka() { lukaAux(_root); }

/*
arg: 0 Node to applied compress function (the root generaly)
Check for each node the associate node of the current Luka's word in our trie
(_words). If nodes aren't the same, we assign the node in the trie to the
current node. Based on BFS.
*/
void Tree::compressAux(std::shared_ptr<Node> &n) {
    if (n == nullptr) {
        return;
    }

    std::shared_ptr<Node> toAssign = _words->Find(n->_value);
    if (toAssign != n) {
        n = toAssign;
    }

    compressAux(n->_leftChild);

    compressAux(n->_rightChild);
}

// Calling aux function to avoid problemes on recursion with the reference
// of a class member
void Tree::Compress() { compressAux(_root); }

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

    system("mkdir -p ../tree");
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
Print the content of luka's trie
*/
void Tree::PrintLukaBTrie() {
    std::cout << "Luka's BTrie:" << std::endl;
    _words->Display();
}

/*
arg: 0 Node to applied compress function (the root of the BDD generaly), 1
Father node of the arg 0, 2 flag that can tell us where we have recovery the
node from Check for each childs of the current node the associate node of
the current Luka's word in our trie (_words). If nodes aren't the same, we
assign the node in the trie to the current node. After that, we need to apply
the deletion rule that say if both childs of a node have the same value
(then are the same because we replaced them by the associate trie value), so
we could remove it ant link there father to there child Based on BFS.
*/ // Find a way to do this prefix
void Tree::compressionBDDAux(std::shared_ptr<Node> &n,
                             std::shared_ptr<Node> &parent, int from) {
    if (n == nullptr) {
        return;
    }

    compressionBDDAux(n->_leftChild, n, LEFT);
    compressionBDDAux(n->_rightChild, n, RIGHT);

    //  then the deletion rule
    if (n->_leftChild != nullptr && n->_rightChild != nullptr &&
        n->_leftChild->_value == n->_rightChild->_value) {
        // update BTrie
        _words->Remove(n->_value);
        auto cpy = n;
        if (from == NOTHING) {
            n = n->_leftChild;
        } else if (from == LEFT) {
            parent->_leftChild = n->_leftChild;
        } else if (from == RIGHT) {
            parent->_rightChild = n->_rightChild;
        }
    }

    // compress init
    std::shared_ptr<Node> toAssign = _words->Find(n->_value);
    if (toAssign != n) {
        n = toAssign;
    }
}

// Calling aux function to avoid problemes on recursion with the reference
// of a class member
void Tree::CompressionBDD() { compressionBDDAux(_root, _root, NOTHING); }

int Tree::CountNode() {
    std::unordered_set<std::shared_ptr<Node>> marked;
    countNodeAux(_root, marked);
    return marked.size();
}

void Tree::countNodeAux(std::shared_ptr<Node> &n,
                        std::unordered_set<std::shared_ptr<Node>> &marked) {
    if (n == nullptr) {
        return;
    }

    marked.insert(n);
    countNodeAux(n->_leftChild, marked);
    countNodeAux(n->_rightChild, marked);
}

/*
[Question 5.18] Melds two BDDs and returns the melded BDD.
Note: recursive and not terminal.
Aval : depth of A in original tree
Bval : depth of B in original tree
*/
std::shared_ptr<Node> Tree::meldAux(std::shared_ptr<Node> &A,
                                    std::shared_ptr<Node> &B, std::string table,
                                    Tree &t) {
    if (A.get() == nullptr or B.get() == nullptr) {
        return nullptr;
    }

    std::shared_ptr<Node> left = nullptr;
    std::shared_ptr<Node> right = nullptr;
    std::string val = "";

    int Aval = A->_value.length();
    int Bval = B->_value.length();

    int i;
    if (Aval == Bval) {
        val = A->_value + " | " + B->_value;

        left = meldAux(A->_leftChild, B->_leftChild, table, t);
        right = meldAux(A->_rightChild, B->_rightChild, table, t);

    } else if (Aval > Bval) {
        int nb = Aval / Bval;
        // std::cout << "nb: " << nb << std::endl;

        std::string tempB_value = B->_value;
        for (i = 0; i < nb - 1; i++) {
            tempB_value += B->_value;
        }
        val = A->_value + " | " + tempB_value;
        left = meldAux(A->_leftChild, B, table, t);
        right = meldAux(A->_rightChild, B, table, t);
    } else if (Bval > Aval) {
        int nb = Bval / Aval;

        std::string tempA_value = A->_value;
        for (i = 0; i < nb - 1; i++) {
            tempA_value += A->_value;
        }
        val = tempA_value + " | " + B->_value;
        left = meldAux(A, B->_leftChild, table, t);
        right = meldAux(A, B->_rightChild, table, t);
    }

    /* Applying table */
    if (table != "") {
        std::vector<std::string> values;
        size_t pos = 0;

        while ((pos = val.find(" | ")) != std::string::npos) {
            std::string token = "";
            token = val.substr(0, pos);

            values.emplace_back(token);
            val.erase(0, pos + std::string(" | ").length());
        }
        values.emplace_back(val);

        val = "";

        for (size_t i(0); i < values[0].length(); ++i) {
            if (values[0][i] == '0' && values[1][i] == '0') {
                val += table[0];
            } else if (values[0][i] == '0' && values[1][i] == '1') {
                val += table[1];
            } else if (values[0][i] == '1' && values[1][i] == '0') {
                val += table[2];
            } else if (values[0][i] == '1' && values[1][i] == '1') {
                val += table[3];
            }
        }
    }
    /* End applying tab*/

    std::shared_ptr<Node> n = newNode(left, right, val);
    // adding word to our BTrie
    t._words->CheckAndInsert(n);
    return n;
}

// Calling Aux
Tree Tree::Meld(Tree &B, std::string table) {
    auto t = Tree();
    t._root = meldAux(_root, B._root, table, t);
    return t;
}