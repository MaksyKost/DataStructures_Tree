#include <iostream>
#include <chrono>
#include <fstream>
#include <functional>
#include "src/avl_tree.h"
#include "src/t_tree.h"
#include "src/splay_tree.h"
#include "src/dancing_tree.h"

using namespace std;
using namespace std::chrono;

template<typename Function>
double measure_time(Function func) {
    auto start = high_resolution_clock::now();
    func();
    auto end = high_resolution_clock::now();
    return duration<double>(end - start).count();
}

int main() {
    const int SIZES[] = {10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000, 100000};
    const int TESTS = 100;
    const int SEEDS[] = {111, 222, 333, 444, 555};

    ofstream resultFile("results/TreePerformance.txt");
    if (!resultFile.is_open()) {
        cerr << "Nie udało się otworzyć pliku." << endl;
        return 1;
    }

    for (int size : SIZES) {
        resultFile << "\n\n==== Testowanie drzew dla rozmiaru: " << size << " elementów ====\n";
        std::cout << "\n\n==== Testowanie drzew dla rozmiaru: " << size << " elementów ====\n";
        struct {
            string name;
            function<TreeStructure*()> create;
        } trees[] = {
            {"AVL Tree", []() { return new AVLTree(); }},
            {"TTree", []() { return new TTree(3); }},
            {"Splay Tree", []() { return new SplayTree(); }},
            {"Dancing Tree", []() { return new DancingTree(); }},
        };

        for (auto& treeInfo : trees) {
            double total_insert = 0.0, total_remove = 0.0, total_search = 0.0;
            int seedCount = 0;

            for (int seed : SEEDS) {
                srand(seed);

                double sum_insert = 0.0, sum_remove = 0.0, sum_search = 0.0;

                for (int t = 0; t < TESTS; ++t) {
                    TreeStructure* tree = treeInfo.create();
                    tree->fillRandom(size);

                    int key = rand() % (size * 10);
                    int val = rand() % 10000;

                    sum_insert += measure_time([&]() { tree->insert(key, val); });
                    int dummy;
                    sum_search += measure_time([&]() { tree->search(key, dummy); });
                    sum_remove += measure_time([&]() { tree->remove(key); });

                    tree->clear();
                    delete tree;
                }

                total_insert += sum_insert / TESTS;
                total_remove += sum_remove / TESTS;
                total_search += sum_search / TESTS;

                seedCount++;
            }

            resultFile << "\n*** Wyniki dla " << treeInfo.name << " ***\n";
            resultFile << "Insert:  " << (total_insert / seedCount) << " s\n";
            resultFile << "Remove:  " << (total_remove / seedCount) << " s\n";
            resultFile << "Search:  " << (total_search / seedCount) << " s\n";
        }
    }

    resultFile.close();
    return 0;
}