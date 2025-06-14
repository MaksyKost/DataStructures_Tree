#ifndef AVL_TREE_H
#define AVL_TREE_H
#include "tree_structure.h"

#include <vector>

struct AVLNode {
    int key;
    int value;
    int height;
    AVLNode* left;
    AVLNode* right;
    AVLNode(int a = 0, int b = 0, int h = 1) : key(a), value(b), height(h), left(nullptr), right(nullptr) {};
};

class AVLTree : TreeStructure{
private:
    AVLNode* root;
    void clear(AVLNode* node);
    AVLNode* insert(AVLNode *node, int key, int value);
    AVLNode* remove(AVLNode *node, int key);
    bool find(AVLNode *node, int key, int& value) const;
    int height(AVLNode* node);
    int balanceFactor(AVLNode* node);
    int max(int a, int b);
    AVLNode* rotateLeft(AVLNode* y);
    AVLNode* rotateRight(AVLNode* x);
    AVLNode* minValueNode(AVLNode* node);
    AVLNode* balance(AVLNode* node);

public:
    AVLTree();
    ~AVLTree();
    void insert(int key, int value);
    void remove(int key);
    bool find(int key, int &value) const;
    void clear();
    void inOrder(std::vector<int>& keys) const;
    AVLNode* getRoot() const { return root; }
};

#endif