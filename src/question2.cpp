#include "head.h"

std::unordered_map<std::string, std::shared_ptr<Node> > _words;

/*
Creating a new empty node
*/
std::shared_ptr<Node> newNode() {

  std::shared_ptr<Node> toReturn = std::make_shared<Node>();

  toReturn->_leftChild = nullptr;
  toReturn->_rightChild = nullptr;
  toReturn->_value = "";

  return toReturn;
}

/*
arg: 0 Initial Tree, 1 Tree to add
Insert a Tree in the Parents childrens
*/
std::shared_ptr<Node> insert(std::shared_ptr<Node> &initTree, std::shared_ptr<Node> treeToAdd) {

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
void createTreeFromTable(std::shared_ptr<Node> &tree, int height, std::string &table) {

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
std::shared_ptr<Node> consArbre(std::string &table) {
  std::shared_ptr<Node> t = newNode();

  createTreeFromTable(t, getTreeHeightFromWidth(table.size()), table);

  return t;
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
arg: 0 stage of the node (call it with 0, a root is alaways as 0), 1 the root
Print all the children of a given node
*/
void printAllChildren(int stage, std::shared_ptr<Node> &n) {

  if (n.get() == nullptr) {
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

void luka(std::shared_ptr<Node> &n) {

  if (n->_leftChild.get() == nullptr) {
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

void compress(std::shared_ptr<Node> &n) {
  if (n.get() == nullptr) {
    return;
  }

  compress(n->_leftChild);

  compress(n->_rightChild);

  if (n != _words[n->_value]) {

    // freeAllChildren(n->_leftChild);
    // freeAllChildren(n->_rightChild);

    n = _words[n->_value];
  }
}

void defineInDot(std::shared_ptr<Node> &n, int height, std::ofstream &f, bool withWords) {

  if (n.get() == nullptr) {
    return;
  }

  const void *address = static_cast<const void *>(n.get());
  std::stringstream ss;
  ss << address;
  std::string name = ss.str();
  name = "_" + name + "_";

  std::string toWrite = "";
  if(withWords) {
    toWrite = name + "[label=\"x" + std::to_string(height) + " [" + n->_value + "]\"];";
  } else {
    if(n->_leftChild.get() == nullptr && n->_rightChild.get() == nullptr) {
      toWrite = name + "[label=\"x" + std::to_string(height) + " [" + n->_value + "]\"];";
    } else {
      toWrite = name + "[label=\"x" + std::to_string(height) + "\"];";
    }
  }
      
  f << toWrite;
  defineInDot(n->_leftChild, height - 1, f, withWords);

  defineInDot(n->_rightChild, height - 1, f, withWords);
}

void linkInDot(std::shared_ptr<Node> &n, std::ofstream &f, std::unordered_set<std::shared_ptr<Node> > &marked) {

  if (n.get() == nullptr) {
    return;
  }

  if ((marked.find(n) != marked.end()) == false) { // if not in the set

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

void dot(std::shared_ptr<Node> &t, bool withWords) {
  // exemple:
  // digraph{a->bb[style=dashed];b[label=\"som1\"];b->c[labal="som1"];a->c;d->c;e->c;e->a;}
  /*for (auto e : _words) {
     std::cout << e.first << std::endl;
   }*/

  system("mkdir ../tree");
  std::ofstream dotFile("../tree/tree.dot");

  std::unordered_set<std::shared_ptr<Node> > marked;

  if (dotFile.is_open()) {

    dotFile << "digraph{";

    int height = getTreeHeightFromWidth(t->_value.size());

    defineInDot(t, height, dotFile, withWords);

    linkInDot(t, dotFile, marked);

    dotFile << "}";

    dotFile.close();
  }

  std::cout << "using dot -Tsvg -o ../tree/tree.svg ../tree/tree.dot"
            << std::endl;

  // creating tree image
  system("dot -Tsvg -o ../tree/tree.svg ../tree/tree.dot");
}
