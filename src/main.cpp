#include "head.h"
#include "tableOfTruth.hpp"

using namespace Devoir;

int main(int argc, char **argv) {

  std::string input = "";

  if (argc == 2) {
    input = argv[1];
  } else {
    // by default the exemple in the subject
    input = "38";
  }

  // We creating our number from our string in base 10
  mpz_class number(input, 10);

  std::cout << "Actual number is : " << number << std::endl;

  // 1<<3 == 2^3 but faster; 1<<n == 2^n;
  // FIX FOR EXEMPLE 201, IF BOTH CHILD GOING TO THE SAME, DEL IT
  // https://en.wikipedia.org/wiki/Binary_decision_diagram

  TableOfTruth t = TableOfTruth();
  std::string tableOfTruth = t.Table(number, 1 << 3);

  // Printing our table of truth
  for (const auto &e : tableOfTruth) {
    std::cout << e << " ";
  }
  std::cout << std::endl;

  std::shared_ptr<Node> tree = consArbre(tableOfTruth);

  std::cout << "Applying Luka" << std::endl;

  luka(tree);

  std::cout << "Compressing" << std::endl;

  compress(tree);

  std::cout << "Creating graph with dot" << std::endl;

  dot(tree, true);

  //printAllChildren(0, tree);
  //printAllChildren(0, tree);
  // Always free after using the tree
  //freeAllChildren(tree);

  exit(0);
}