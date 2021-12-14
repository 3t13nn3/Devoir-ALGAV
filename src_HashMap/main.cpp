#include <chrono>
#include <cmath>
#include <cstdlib>
#include <map>
#include <thread>

#include "TableOfTruth.hpp"
#include "Tree.hpp"

void plot(int nbVariable, int nbTest = 100000) {
    int lenghtBinaryNumber = (1 << nbVariable);

    system("mkdir -p ../plot");

    std::string filename = std::to_string(nbVariable) + "_variables";
    std::ofstream plotFile("../plot/" + filename + ".csv");
    std::map<int, int> count;
    long long int numberOfVariable = (pow(2, pow(2, nbVariable)));
    std::string tableOfTruth = "";

    double ROBDDtime = 0;
    auto start = std::chrono::high_resolution_clock::now();

    if (nbVariable < 5) {
        std::string toFind(lenghtBinaryNumber, '1');
        for (long long int i(0); i < numberOfVariable; ++i) {
            mpz_class number(std::to_string(i), 10);
            ex1::TableOfTruth tof = ex1::TableOfTruth();
            tableOfTruth = tof.Table(number, lenghtBinaryNumber);

            /* construct robdd */
            auto s = std::chrono::high_resolution_clock::now();

            ex2ex3::Tree myTree = ex2ex3::Tree();
            myTree.ConsArbre(tableOfTruth);
            myTree.Luka();
            myTree.CompressionBDD();

            auto e = std::chrono::high_resolution_clock::now() - s;
            ROBDDtime +=
                std::chrono::duration_cast<std::chrono::microseconds>(e)
                    .count();

            int nodeCount = myTree.CountNode();
            count[nodeCount]++;
        }

        auto elapsed = std::chrono::high_resolution_clock::now() - start;
        double microseconds =
            std::chrono::duration_cast<std::chrono::microseconds>(elapsed)
                .count();
        std::cout << "| NbVariable: " << nbVariable
                  << "\t| NbSamples: " << numberOfVariable
                  << "\t| UniqueSize: " << count.size()
                  << "\t| Time: " << microseconds / 1000000.f << " s"
                  << "\t| TimePerROBDD: " << (ROBDDtime / numberOfVariable)
                  << " µs\t|" << std::endl;

        for (const auto &e : count) {
            plotFile << std::to_string(e.first) << "\t"
                     << std::to_string(e.second) << "\n";
        }

    } else {
        std::unordered_set<std::string> pickedNumbers;
        srand(time(NULL));

        for (int i(0); i < nbTest; ++i) {
            std::string random(lenghtBinaryNumber, '1');
            for (size_t i(0); i < random.size(); ++i) {
                random[i] = std::to_string(rand() % 2)[0];
            }
            while (pickedNumbers.find(random) != pickedNumbers.end()) {
                for (size_t i(0); i < random.size(); ++i) {
                    random[i] = std::to_string(rand() % 2)[0];
                }
            }

            pickedNumbers.insert(random);
            mpz_class number(random, 10);
            ex1::TableOfTruth tof = ex1::TableOfTruth();
            tableOfTruth = tof.Table(number, lenghtBinaryNumber);

            /* construct robdd */
            auto s = std::chrono::high_resolution_clock::now();
            ex2ex3::Tree myTree = ex2ex3::Tree();
            myTree.ConsArbre(tableOfTruth);
            myTree.Luka();
            myTree.CompressionBDD();
            auto e = std::chrono::high_resolution_clock::now() - s;
            ROBDDtime +=
                std::chrono::duration_cast<std::chrono::microseconds>(e)
                    .count();

            int nodeCount = myTree.CountNode();
            count[nodeCount]++;
        }

        auto elapsed = std::chrono::high_resolution_clock::now() - start;
        double microseconds =
            std::chrono::duration_cast<std::chrono::microseconds>(elapsed)
                .count();
        std::cout << "| NbVariable: " << nbVariable
                  << "\t| NbSamples: " << nbTest
                  << "\t| UniqueSize: " << count.size()
                  << "\t| Time: " << microseconds / 1000000.f << " s"
                  << "\t| TimePerROBDD: " << (ROBDDtime / nbTest) << " µs\t|"
                  << std::endl;

        if (nbVariable < 10) {
            for (const auto &e : count) {
                plotFile << std::to_string((long int)(e.first)) << "\t"
                         << std::to_string((long int)e.second *
                                           (pow(2, lenghtBinaryNumber)) /
                                           nbTest)
                         << "\n";
            }
        } else {
            /* need to avoid normal type because with 10 variables can't handle
             */
            mpz_class nb;
            mpz_ui_pow_ui(nb.get_mpz_t(), 2, lenghtBinaryNumber);
            mpz_class test(std::to_string(nbTest), 10);
            for (const auto &e : count) {
                mpz_class second(std::to_string(e.second), 10);
                mpz_class actual(nb);
                actual *= second;
                actual /= test;
                plotFile << std::to_string((long int)(e.first)) << "\t"
                         << actual << "\n";
            }
        }
    }

    plotFile.close();
    // [0:*] to start at [0:*]
    system(("gnuplot --persist -e 'set xrange [*:*];set grid;set term svg;set "
            "output \"../plot/" +
            filename + ".svg\";set xlabel \"ROBDD node count for " +
            std::to_string(nbVariable) +
            " variables\" font \",12\";set ylabel \"Number of Boolean "
            "functions\" font \",12\";plot \"../plot/" +
            filename +
            ".csv\" using 1:2 with lp pt 7 lt 2 linewidth 2 title \"\""
            "' 2>/dev/null ")
               .c_str());
}

int main(int argc, char **argv) {
    std::string input = "";

    if (argc == 2) {
        input = argv[1];
    } else {
        // by default the exemple in the subject
        input = "38";
    }

    /* PLOT */
    // plot(5);
    //for (int i(1); i <= 10; ++i) plot(i);

    // We creating our number from our string in base 10
    mpz_class number(input, 10);
    std::cout << "Actual number is : " << number << std::endl;

    // Question 1
    ex1::TableOfTruth tof = ex1::TableOfTruth();
    // 1<<n == 2^n;
    std::string tableOfTruth = tof.Table(number, 1 << 3);
    std::string tableOfTruthCpy = tableOfTruth;
    // std::cout << tableOfTruth << std::endl;

    // Question 2
    ex2ex3::Tree tree1 = ex2ex3::Tree();
    tree1.ConsArbre(tableOfTruth);
    tree1.Luka();

    /*std::cout << "Creating init.dot" << std::endl;
    tree1.Dot("init");*/

    tree1.CompressionBDD();
    // tree1.PrintLukaMap();
    // tree1.PrintAllChildren();

    std::cout << "Creating compressed.dot" << std::endl;
    tree1.Dot("compressed");

    tof = ex1::TableOfTruth();  //// 61152
    tableOfTruth = tof.Table(mpz_class("28662", 10), 1 << 4);

    ex2ex3::Tree tree2 = ex2ex3::Tree();
    tree2.ConsArbre(tableOfTruth);
    tree2.Luka();
    // tree2.Compress();
    tree2.CompressionBDD();
    tree2.Dot("compressed2");

    // Table of truth of AND '0001'
    tree1.FusionBDD(tree2);

    tree1.Luka();

    tree1.CompressionBDD();

    tree1.Dot("Fusion");

    exit(0);
}
