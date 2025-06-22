#ifndef SPLAY_TREE_H
#define SPLAY_TREE_H

struct SplayTreeNode {
    int key;
    int value;
    SplayTreeNode* left;
    SplayTreeNode* right;

    SplayTreeNode(int k, int val) : key(k), value(val), left(nullptr), right(nullptr) {}
};

class SplayTree {
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
    ~SplayTree();

    // PODSTAWOWE METODY
    void insert(int key, int value);
    bool search(int key, int& value); // search w Splay Tree też przesuwa znaleziony węzeł w kierunku korzenia 
    void remove(int key);

    void clear();
    void display() const;
};

#endif