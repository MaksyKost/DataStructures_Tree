#include "interfaces/gui.h"
#include "interfaces/cli.h"
#include <iostream>
#include <string>

#include "src/tree_structure.h" 
#include "src/avl_tree.h"       
#include "src/dancing_tree.h"   
#include "src/splay_tree.h"     

#include <iostream>
#include <vector>
#include <algorithm> 

// Helper function to test a given tree implementation
void testTree(TreeStructure* tree, const std::string& treeName) {
    std::cout << "--- Testing " << treeName << " ---" << std::endl;

    std::cout << "Initial " << treeName << " structure: ";
    tree->displayStructure();

    std::cout << "Inserting elements..." << std::endl;
    std::vector<std::pair<int, int>> elements_to_insert = {
        {50, 500}, {30, 300}, {70, 700}, {20, 200}, {40, 400},
        {60, 600}, {80, 800}, {10, 100}, {55, 550}, {75, 750},
        {5, 50}, {15, 150}, {35, 350}, {45, 450}
    };

    for (const auto& p : elements_to_insert) {
        std::cout << "Inserting (" << p.first << ":" << p.second << ")\n";
        tree->insert(p.first, p.second);
        std::cout << "Tree after insert (" << p.first << "): ";
        tree->display();
        std::cout << "Tree after insert (" << p.first << ") (Structure): ";
        tree->displayStructure();
    }
    std::cout << "\nFinal " << treeName << " after all insertions (in-order traversal): ";
    tree->display();
    std::cout << "Final " << treeName << " after all insertions (Structure): ";
    tree->displayStructure();
    std::cout << std::endl;

    std::cout << "Searching for elements..." << std::endl;
    std::vector<int> keys_to_search = {40, 90, 70, 10, 1, 50};
    for (int key : keys_to_search) {
        int found_value = 0;
        std::cout << "Searching for key " << key << ": ";
        if (tree->search(key, found_value)) {
            std::cout << "Found with value " << found_value << std::endl;
        } else {
            std::cout << "Not found." << std::endl;
        }
        
        std::cout << treeName << " after search (" << key << ") (Structure): ";
        tree->displayStructure(); // <--- САМОЕ ВАЖНОЕ
    }
    std::cout << std::endl;

    std::cout << "Removing elements..." << std::endl;
    std::vector<int> keys_to_remove = {20, 70, 50, 5, 80, 100}; // 100 is not in the tree
    for (int key : keys_to_remove) {
        std::cout << "Removing " << key << "\n";
        tree->remove(key);
        std::cout << "Tree after remove (" << key << ") (In-order): ";
        tree->display();
        std::cout << "Tree after remove (" << key << ") (Structure): ";
        tree->displayStructure();
    }
    std::cout << "\nFinal " << treeName << " after all removals: ";
    tree->display();
    std::cout << std::endl;

    // Optional: Test inserting existing keys to update value
    std::cout << "Testing update functionality (inserting existing key with new value):" << std::endl;
    int existing_key = 30;
    int new_value = 3030;
    std::cout << "Inserting (" << existing_key << ":" << new_value << ") (should update existing 30)." << std::endl;
    tree->insert(existing_key, new_value);
    std::cout << "Tree after update: ";
    tree->display();
    int updated_value = 0;
    if (tree->search(existing_key, updated_value)) {
        std::cout << "Updated key " << existing_key << " now has value " << updated_value << std::endl;
    } else {
        std::cout << "Error: Updated key " << existing_key << " not found after update." << std::endl;
    }
    std::cout << std::endl;


    std::cout << "--- " << treeName << " Test Finished ---" << std::endl << std::endl;
}

int main(int argc, char* argv[]) {
    // Test AVL Tree
    AVLTree avlTree;
    testTree(&avlTree, "AVL Tree");

    // Test Dancing Tree
    DancingTree dancingTree;
    testTree(&dancingTree, "Dancing Tree");

    // Test Splay Tree
    SplayTree splayTree;
    testTree(&splayTree, "Splay Tree");


/*
    std::string mode = "gui";
    if (argc > 1) {
        mode = argv[1];
    }

    if (mode == "gui") {
        // Uruchomienie trybu graficznego
        GUI gui;
        if (!gui.init()) {
            std::cerr << "Błąd inicjalizacji GUI (SDL)." << std::endl;
            std::cin.get(); // Zatrzymaj program, aby zobaczyć komunikat
            return 1;
        }
        Board board;
        gui.run(board);
        gui.close();
    }
    else if (mode == "cli") {
        // Uruchomienie trybu konsolowego

    }
    else {
        std::cerr << "Nieznany tryb: " << mode << ". Dostępne tryby: cli, gui." << std::endl;
        return 1;
    }*/
    return 0;
}