#include "head.h"

std::unordered_map<std::string, Node *> _words;

/*
Creating a new empty node
*/
Node *newNode() {

  Node *toReturn = new Node;

  toReturn->_leftChild = nullptr;
  toReturn->_rightChild = nullptr;
  toReturn->_value = "";

  return toReturn;
}

/*
arg: 0 Initial Tree, 1 Tree to add
Insert a Tree in the Parents childrens
*/
Node *insert(Node *initTree, Node *treeToAdd) {

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

/*
arg: 0 Initial Tree, 1 Height of the tree, 2 table of truth
Create a Tree of a Height "height", based on DFS, putting the table of truth
value at leafs
*/
void createTreeFromTable(Node *tree, int height, std::string &table) {

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
Node *consArbre(std::string table) {
  Node *t = newNode();

  createTreeFromTable(t, getTreeHeightFromWidth(table.size()), table);

  return t;
}

/*
arg: 0 Root node
Free all the children of a given node
*/
void freeAllChildren(Node *&n) {

  if (n == nullptr) {
    return;
  }

  freeAllChildren(n->_leftChild);

  freeAllChildren(n->_rightChild);

  n = nullptr;

  if (n == nullptr) { // needed because a same node could be deleted and we
                      // don't want this
    delete n;
  }
}

/*
arg: 0 stage of the node (call it with 0, a root is alaways as 0), 1 the root
Print all the children of a given node
*/
void printAllChildren(int stage, Node *n) {

  if (n == nullptr) {
    return;
  }

  printAllChildren(stage + 1, n->_leftChild);

  printAllChildren(stage + 1, n->_rightChild);

  int i;
  for (i = 0; i < stage; i++) {
    std::cout << "- - - -\t\t";
  }
  std::cout << "[" << stage << "]"
            << "Node: ";
  std::cout << n->_value << std::endl;
}

void luka(Node *n) {

  if (n->_leftChild == nullptr) {
    if (_words.find(n->_value) == _words.end()) {
      // not found
      _words[n->_value] = n;
      std::cout << n->_value << std::endl;
    }

    return;
  }

  luka(n->_leftChild);

  luka(n->_rightChild);

  n->_value = n->_leftChild->_value + n->_rightChild->_value;

  // adding word to our map
  //_words[n->_value] = n;
  if (_words.find(n->_value) == _words.end()) {
    // not found
    _words[n->_value] = n;
    std::cout << n->_value << std::endl;
  }
}

void compress(Node *&n) {
  if (n == nullptr) {
    return;
  }

  /*
    if (n->_leftChild != nullptr) {
      if (n->_leftChild != _words[n->_leftChild->_value]) {
        freeAllChildren(n->_leftChild);
        //   delete n->_leftChild;
        // n->_leftChild = new Node;
        insert(n, _words[n->_leftChild->_value]);
      }
    }
    if (n->_rightChild != nullptr) {
      if (n->_rightChild != _words[n->_rightChild->_value]) {
        // freeAllChildren(n->_rightChild);
        //   delete n->_rightChild;
        // n->_leftChild = new Node;
        n->_rightChild = _words[n->_rightChild->_value];
      }
    }
  */
  compress(n->_leftChild);

  compress(n->_rightChild);

  if (n != _words[n->_value]) {
    // std::cout << (_words[n->_value])->_value << std::endl;

    n = _words[n->_value];
    n->_leftChild = nullptr;
    n->_rightChild = nullptr;
    /*freeAllChildren(n->_leftChild);
    freeAllChildren(n->_rightChild);*/
  }
  /*
    if (n != _words[n->_value]) {
      // std::cout << (_words[n->_value])->_value << std::endl;

      freeAllChildren(n);

      // n = _words[n->_value];
    }*/
}

void defineInDot(Node *n, int height, std::ofstream &f) {

  if (n == nullptr) {
    return;
  }

  const void *address = static_cast<const void *>(n);
  std::stringstream ss;
  ss << address;
  std::string name = ss.str();
  name = "_" + name + "_";

  std::string toWrite =
      name + "[label=\"x" + std::to_string(height) + " [" + n->_value + "]\"];";
  f << toWrite;
  defineInDot(n->_leftChild, height - 1, f);

  defineInDot(n->_rightChild, height - 1, f);
}

void linkInDot(Node *n, std::ofstream &f) {

  if (n == nullptr) {
    return;
  }

  const void *address = static_cast<const void *>(n);
  std::stringstream ss;
  ss << address;
  std::string father = ss.str();
  father = "_" + father + "_";

  if (n->_leftChild != nullptr) {

    const void *address = static_cast<const void *>(n->_leftChild);
    std::stringstream ss;
    ss << address;
    std::string child = ss.str();
    child = "_" + child + "_";

    std::string toWrite = father + "->" + child + "[style=dashed];";
    f << toWrite;
  }

  if (n->_rightChild != nullptr) {

    const void *address = static_cast<const void *>(n->_rightChild);
    std::stringstream ss;
    ss << address;
    std::string child = ss.str();
    child = "_" + child + "_";

    std::string toWrite = father + "->" + child + ";";
    f << toWrite;
  }

  linkInDot(n->_leftChild, f);

  linkInDot(n->_rightChild, f);
}

void dot(Node *t) {
  // exemple:
  // digraph{a->bb[style=dashed];b[label=\"som1\"];b->c[labal="som1"];a->c;d->c;e->c;e->a;}
  /*for (auto e : _words) {
     std::cout << e.first << std::endl;
   }*/

  system("mkdir ../tree");
  std::ofstream dotFile("../tree/tree.dot");

  if (dotFile.is_open()) {

    dotFile << "digraph{";

    int height = getTreeHeightFromWidth(t->_value.size());

    defineInDot(t, height, dotFile);

    linkInDot(t, dotFile);

    dotFile << "}";

    dotFile.close();
  }

  std::cout << "using dot -Tsvg -o ../tree/tree.svg ../tree/tree.dot"
            << std::endl;

  // creating tree image
  system("dot -Tsvg -o ../tree/tree.svg ../tree/tree.dot");
}
