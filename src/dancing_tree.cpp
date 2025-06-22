#include "dancing_tree.h"
#include <iostream>

// funkcja pomocnicza dla usuwania drzewa
void DancingTree::deleteSubtree(DancingTreeNode* node) {
        if (!node) return;
        deleteSubtree(node->left);
        deleteSubtree(node->right);
        delete node;
    }

DancingTree::DancingTree() : root(nullptr) {}

DancingTree::~DancingTree() {
    deleteSubtree(root);
}

// 
DancingTreeNode* rotateLeft(DancingTreeNode* node) {
    if (!node || !node->right) return node;
    DancingTreeNode* newRoot = node->right;
    node->right = newRoot->left;
    newRoot->left = node;
    return newRoot;
}

DancingTreeNode* rotateRight(DancingTreeNode* node) {
    if (!node || !node->left) return node;
    DancingTreeNode* newRoot = node->left;
    node->left = newRoot->right;
    newRoot->right = node;
    return newRoot;
}

void DancingTree::insert(int key, int value) {
    if (!root) {
        root = new DancingTreeNode{key, value};
        return;
    }

    DancingTreeNode* parent = nullptr;
    DancingTreeNode* current = root;
    while (current) {
        parent = current;
        if (key < current->key)
            current = current->left;
        else if (key > current->key)
            current = current->right;
        else {
            current->value = value;
            return;
        }
    }
    if (key < parent->key)
        parent->left = new DancingTreeNode(key, value);
    else
        parent->right = new DancingTreeNode(key, value);

    // "dancing" logic
    if (key % 2 == 0) {
        root = rotateRight(root);
    } else {
        root = rotateLeft(root);
    }
}

void DancingTree::remove(int key) {
    DancingTreeNode* parent = nullptr;
    DancingTreeNode* current = root;

    while (current && current->key != key) {
        parent = current;
        current = (key < current->key) ? current->left : current->right;
    }
    if (!current) return;

    if (current->left && current->right) {
        DancingTreeNode* replaceParent = current;
        DancingTreeNode* replace = current->left;
        while (replace->right) {
            replaceParent = replace;
            replace = replace->right;
        }
        current->key = replace->key;
        current->value = replace->value;
        current = replace;
        parent = replaceParent;
    }

    DancingTreeNode* child = (current->left) ? current->left : current->right;
    if (!parent) {
        root = child;
    } else if (parent->left == current) {
        parent->left = child;
    } else {
        parent->right = child;
    }
    delete current;

    // "dancing" logic
    if (key % 2 == 0) {
        root = rotateLeft(root);
    } else {
        root = rotateRight(root);
    }
}

bool DancingTree::search(int key, int& value) {
    DancingTreeNode* current = root;
    while (current) {
        if (current->key == key) {
            value = current->value;
            return true;
        }
        current = (key < current->key) ? current->left : current->right;
    }
    return false;
}

void printInOrder(const DancingTreeNode* node) {
    if (!node) return;
    printInOrder(node->left);
    std::cout << "(" << node->key << ":" << node->value << ") ";
    printInOrder(node->right);
}

void DancingTree::display() const {
    printInOrder(root);
    std::cout << std::endl;
}