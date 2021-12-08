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
    std::string tableOfTruth = tof.Table(number, 1 << 2);
    std::string tableOfTruthCpy = tableOfTruth;
    // std::cout << tableOfTruth << std::endl;

    auto start = std::chrono::high_resolution_clock::now();
    // Question 2
    ex2ex3::Tree tree1 = ex2ex3::Tree();
    tree1.ConsArbre(tableOfTruth);
    tree1.Luka();
    tree1.CompressionBDD();
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    long long microseconds =
        std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    std::cout << microseconds << "µs" << std::endl;
    // tree1.PrintLukaMap();
    // tree1.PrintAllChildren();

    std::cout << "Creating compressed.dot" << std::endl;
    tree1.Dot("compressed");
/*
    tof = ex1::TableOfTruth();  //// 61152
    tableOfTruth = tof.Table(mpz_class("28662", 10), 1 << 4);

    ex2ex3::Tree tree2 = ex2ex3::Tree();
    tree2.ConsArbre(tableOfTruth);
    tree2.Luka();
    // tree2.Compress();
    tree2.CompressionBDD();
    tree2.Dot("compressedBDD");

    // Table of truth of AND '0001'
    tree1.FusionBDD(tree2);

    tree1.Luka();

    tree1.CompressionBDD();

    tree1.Dot("Fusion");
*/
    exit(0);
}
