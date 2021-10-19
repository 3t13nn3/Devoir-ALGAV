#include "head.h"

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
  std::string tableOfTruth = table(number, 1 << 3);

  // Printing our table of truth
  for (const auto &e : tableOfTruth) {
    std::cout << e << " ";
  }
  std::cout << std::endl;

  Node *tree = consArbre(tableOfTruth);

  std::cout << "Applying Luka" << std::endl;

  luka(tree);

  std::cout << "Compressing" << std::endl;

  compress(tree);

  std::cout << "Creating graph with dot" << std::endl;

  dot(tree);

  // printAllChildren(0, tree);
  printAllChildren(0, tree);
  // Always free after using the tree
  freeAllChildren(tree);

  exit(0);
}