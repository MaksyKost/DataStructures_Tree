#include "splay_tree.h"
#include <iostream>

// --- Funkcje private --- //

void SplayTree::clear(SplayTreeNode* node) {
    if (node == nullptr) return;
    clear(node->left);
    clear(node->right);
    delete node;
}

void SplayTree::clear() {
    clear(root);
    root = nullptr;
}

// lewy obrót
SplayTreeNode* SplayTree::rotateLeft(SplayTreeNode* node) {
    SplayTreeNode* newRoot = node->right;
    node->right = newRoot->left;
    newRoot->left = node;
    return newRoot;
}

// prawy obrót
SplayTreeNode* SplayTree::rotateRight(SplayTreeNode* node) {
    SplayTreeNode* newRoot = node->left;
    node->left = newRoot->right;
    newRoot->right = node;
    return newRoot;
}

SplayTreeNode* SplayTree::splay(SplayTreeNode* node, int key) {
    // drzewo puste/klucz w korzeniu
    if (node == nullptr || node->key == key) {
        return node;
    }

    if (key < node->key) {
        if (node->left == nullptr) {
            return node;
        }
        if (key < node->left->key) { // Zig-Zig (lewo-lewo)
            node->left->left = splay(node->left->left, key);
            node = rotateRight(node); // pierwszy obrót na górę
        } else if (key > node->left->key) { // Zig-Zag (lewo-prawo)
            node->left->right = splay(node->left->right, key);
            if (node->left->right != nullptr) {
                node->left = rotateLeft(node->left); // pierwszy obrót (lewogo potomka)
            }
        }

        // drugi obrót
        return (node->left == nullptr) ? node : rotateRight(node);

    } else {
        if (node->right == nullptr) {
            return node;
        }
        if (key > node->right->key) { // Zig-Zig (prawo-prawo)
            node->right->right = splay(node->right->right, key);
            node = rotateLeft(node); // pierwszy obrót na górę
        } else if (key < node->right->key) { // Zig-Zag (prawo-lewo)
            node->right->left = splay(node->right->left, key);
            if (node->right->left != nullptr) {
                node->right = rotateRight(node->right); // pierwszy obrót (prawego potomka)
            }
        }

        // drugi obrót
        return (node->right == nullptr) ? node : rotateLeft(node);
    }
}

SplayTreeNode* SplayTree::minValueNode(SplayTreeNode* node) {
    SplayTreeNode* current = node;
    while (current && current->left != nullptr) {
        current = current->left;
    }
    return current;
}

void SplayTree::printInOrder(const SplayTreeNode* node) const {
    if (node == nullptr) return;
    printInOrder(node->left);
    std::cout << node->key << " ";
    printInOrder(node->right);
}

// --- metody public --- //

// konstruktor
SplayTree::SplayTree() : root(nullptr) {}

// destruktor
SplayTree::~SplayTree() {
    clear();
}

void SplayTree::clear() {
    clear(root);
    root = nullptr;
}

void SplayTree::insert(int key, int value) {
    if (root == nullptr) {
        root = new SplayTreeNode(key, value);
        return;
    }

    // najpierw splay. jeśli już tam jest ten klucz, stanie się korzeniem.
    // jeśli go nie ma, korzeniem będzie ostatni odwiedzony węzeł przed nullptr.
    root = splay(root, key);

    if (key == root->key) {
        // klucz już istnieje, aktualizujemy wartość
        root->value = value;
        return;
    }

    // nie ma klucza, tworzymy nowy wezęł
    SplayTreeNode* newNode = new SplayTreeNode(key, value);

    if (key < root->key) {
        newNode->right = root;
        newNode->left = root->left;
        root->left = nullptr; // stary korzeń ma puste lewe poddrzewo
    } else { // key > root->key
        newNode->left = root;
        newNode->right = root->right;
        root->right = nullptr; // stary korzeń ma puste prawe poddrzewo
    }
    root = newNode; // nowy węzeł staje się korzeniem
}

bool SplayTree::search(int key, int& value) {
    root = splay(root, key);

    if (root != nullptr && root->key == key) {
        value = root->value;
        return true;
    }
    return false;
}

void SplayTree::remove(int key) {
    if (root == nullptr) return;

    root = splay(root, key); // Przesuwamy węzeł do korzenia

    if (key != root->key) {
        // nie ma węzła z takim kluczem
        return;
    }

    // jeśli jest i został korzeniem
    SplayTreeNode* temp = root;
    if (root->left == nullptr) {
        root = root->right; // nie ma lewego poddrzewa, prawe staje się korzeniem
    } else {
        // jest lewe poddrzewo -> splay(największy element lewego poddrzewa, korzeń poddrzewa)
        // dołączamy prawe poddrzewo do tego nowego korzenia
        SplayTreeNode* newRootLeftSubtree = splay(root->left, key);
        newRootLeftSubtree->right = root->right;
        root = newRootLeftSubtree;
    }
    delete temp; 
}

void SplayTree::display() const {
    printInOrder(root);
    std::cout << std::endl;
}

void printPreOrderSplay(const SplayTreeNode* node) {
    if (!node) return;
    std::cout << "(" << node->key << ":" << node->value << ") ";
    printPreOrderSplay(node->left);
    printPreOrderSplay(node->right);
}

void SplayTree::displayStructure() const {
    std::cout << "Pre-order traversal (Root, Left, Right): ";
    printPreOrderSplay(root);
    std::cout << std::endl;
}