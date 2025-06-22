#ifndef SPLAY_TREE_H
#define SPLAY_TREE_H

#include <iostream>
#include "tree_structure.h"

struct SplayTreeNode {
    int key;
    int value;
    SplayTreeNode* left;
    SplayTreeNode* right;

    SplayTreeNode(int k, int val) : key(k), value(val), left(nullptr), right(nullptr) {}
};

class SplayTree : public TreeStructure{
private:
    SplayTreeNode* root;

    // przenosi węzeł z danym kluczem do korzenia drzewa
    SplayTreeNode* splay(SplayTreeNode* node, int key);

    // FUNKCJE WSPOMAGAJĄCE //
    // rotacja
    SplayTreeNode* rotateLeft(SplayTreeNode* node);
    SplayTreeNode* rotateRight(SplayTreeNode* node);

    // rekurencyjne usuwania poddrzewa
    void clear(SplayTreeNode* node);

    // wyszukiwanie węzła z minimalnym kluczem w poddrzewie
    SplayTreeNode* minValueNode(SplayTreeNode* node);

    // Funkcja pomocnicza do przechodzenia
    void printInOrder(const SplayTreeNode* node) const;

public:
    // konstruktor/destruktor
    SplayTree();
    ~SplayTree() override;

    // PODSTAWOWE METODY
    virtual void insert(int key, int value) override;
    virtual bool search(int key, int& value) override; // search w Splay Tree też przesuwa znaleziony węzeł w kierunku korzenia 
    virtual void remove(int key) override;

    void clear();
    virtual void display() const override;
    virtual void displayStructure() const override;
};

#endif