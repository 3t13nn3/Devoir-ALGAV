#include <chrono>
#include <thread>

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
    // 1<<n == 2^n;
    std::string tableOfTruth = tof.Table(number, 1 << 3);
    std::string tableOfTruthCpy = tableOfTruth;
    std::cout << tableOfTruth << std::endl;
    
    // Question 2
    ex2ex3::Tree tree1 = ex2ex3::Tree();
    tree1.ConsArbre(tableOfTruth);
    std::cout << "Applying Luka on tree1" << std::endl;
    tree1.Luka();
    tree1.PrintLukaMap();
    tree1.PrintAllChildren();
    // std::cout << "Creating init.dot" << std::endl;
    tree1.Dot("init");

    std::cout << "Compressing" << std::endl;
    tree1.Compress();
    tree1.PrintAllChildren();
    std::cout << "Creating compressed.dot" << std::endl;
    tree1.Dot("compressed");

    // Question 3
    // working on a brand new tree, construct from the same table of truth
    ex2ex3::Tree tree2 = ex2ex3::Tree();
    tree2.ConsArbre(tableOfTruthCpy);

    std::cout << "Applying Luka on tree2" << std::endl;
    tree2.Luka();
    tree2.PrintLukaMap();

    std::cout << "Compressing further" << std::endl;
    tree2.CompressionBDD();
    tree2.PrintAllChildren();

    std::cout << "Creating compressedBDD.dot" << std::endl;
    tree2.Dot("compressedBDD");
    exit(0);
}
