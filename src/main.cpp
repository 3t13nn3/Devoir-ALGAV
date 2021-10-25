//#include "head.h"
#include "TableOfTruth.hpp"
#include "Tree.hpp"

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

  // Question 1

  ex1::TableOfTruth tof = ex1::TableOfTruth();

  // 1<<3 == 2^3 but faster; 1<<n == 2^n;
  // FIX FOR EXEMPLE 201, IF BOTH CHILD GOING TO THE SAME, DEL IT
  // https://en.wikipedia.org/wiki/Binary_decision_diagram
  std::string tableOfTruth = tof.Table(number, 1 << 3);

  std::cout << tableOfTruth << std::endl;

  // Question 2

  ex2::Tree tree = ex2::Tree();

  tree.ConsArbre(tableOfTruth);

  std::cout << "Applying Luka" << std::endl;
  tree.Luka();

  std::cout << "Compressing" << std::endl;
  tree.Compress();

  std::cout << "Creating graph with dot" << std::endl;
  tree.Dot();

  tree.PrintAllChildren();

  tree.PrintLukaMap();

  exit(0);
}
