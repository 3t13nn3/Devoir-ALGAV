#include <chrono>
#include <thread>
#include <map>
#include <cmath>
#include <cstdlib>

#include "TableOfTruth.hpp"
#include "Tree.hpp"

void plot(int nbVariable, int nbTest = 100000) {
    int max = (1 << nbVariable);
    
    system("mkdir ../plot");

    std::string filename = std::to_string(nbVariable) + "_variables";    
    std::ofstream plotFile("../plot/" + filename + ".csv");
    std::map<int, int> count;

    std::string tableOfTruth = "";

    if (nbVariable < 5 ) {
        std::string toFind(max, '1');
        for (int i(0); ; ++i){
            mpz_class number(std::to_string(i), 10);
            ex1::TableOfTruth tof = ex1::TableOfTruth();
            tableOfTruth = tof.Table(number, max);
            if(!toFind.compare(0,max,tableOfTruth)) {
                break;
            }
            ex2ex3::Tree myTree = ex2ex3::Tree();

            myTree.ConsArbre(tableOfTruth);
            myTree.Luka(); 
            myTree.CompressionBDD();
            int nodeCount = myTree.CountNode();
            count[nodeCount]++;
                        std::cout << i << std::endl;
        }
        std::cout << (pow(2, max)) << std::endl;
        // we know 1111 have only one nodes and we break just befor then assigning it
        count[1]++;
        for(const auto &e : count) {
            plotFile << std::to_string(e.first) << "\t" << std::to_string(e.second) << "\n";
        }
    } else {
        std::unordered_set<int> pickedNumbers;
        srand (time(NULL));
        
        for(int i(0); i < nbTest; ++i) {
            
            long int random = rand() % (long int)((pow(2, max)) + 1);
            while(pickedNumbers.find(random) != pickedNumbers.end()) {
                random = rand() % (long int)((pow(2, max)) + 1);
            }
            pickedNumbers.insert(random);
            mpz_class number(std::to_string(random), 10);
            ex1::TableOfTruth tof = ex1::TableOfTruth();
            tableOfTruth = tof.Table(number, max);
            ex2ex3::Tree myTree = ex2ex3::Tree();

            myTree.ConsArbre(tableOfTruth);
            myTree.Luka(); 
            myTree.CompressionBDD();
            int nodeCount = myTree.CountNode();
            count[nodeCount]++;

        }
//std::to_string((long int)e.second * (pow(2, max)) / nbTest)

        for(const auto &e : count) {
            plotFile << std::to_string((long int)(e.first)) << "\t" << std::to_string((long int)e.second * (pow(2, max)) / nbTest) << "\n";
        }
        
    }

    
    plotFile.close();
    system(("gnuplot --persist -e 'set term png;set output \"../plot/" + filename + ".png\";set xlabel \"ROBDD node count for " + std::to_string(nbVariable) + " variables\" font \",12\";set ylabel \"Number of Boolean functions\" font \",12\";plot \"../plot/" + filename + ".csv\" using 1:2 with lp pt 7 title \"\"""'").c_str());
}

int main(int argc, char **argv) {
    std::string input = "";

    if (argc == 2) {
        input = argv[1];
    } else {
        // by default the exemple in the subject
        input = "38";
    }
    plot(2);
/*
    for(int i(0); i<=10; ++i)
        plot(i);*/
/*
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
    tree1.PrintLukaMap();
    // tree1.PrintAllChildren();

    std::cout << "Creating compressed.dot" << std::endl;
    tree1.Dot("compressed");
 */
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
