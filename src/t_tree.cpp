#include "t_tree.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

TTreeNode::TTreeNode() : left(nullptr), right(nullptr), parent(nullptr) {}

bool TTreeNode::insertKey(int key) {
    if (std::find(keys.begin(), keys.end(), key) != keys.end())
        return false;
    keys.push_back(key);
    std::sort(keys.begin(), keys.end());
    return true;
}

bool TTreeNode::containsKey(int key) const {
    return std::find(keys.begin(), keys.end(), key) != keys.end();
}

bool TTreeNode::removeKey(int key) {
    auto it = std::find(keys.begin(), keys.end(), key);
    if (it != keys.end()) {
        keys.erase(it);
        return true;
    }
    return false;
}

TTree::TTree(size_t maxKeysPerNode)
    : root(nullptr), maxKeys(maxKeysPerNode) {}

TTree::~TTree() {
    clear();
}

void TTree::insert(int value) {
    root = insertRecursive(root, value);
}

TTreeNode* TTree::insertRecursive(TTreeNode* node, int key) {
    if (!node) {
        node = new TTreeNode();
        node->insertKey(key);
        return node;
    }

    if (key < node->keys.front()) {
        node->left = insertRecursive(node->left, key);
        node->left->parent = node;
    } else if (key > node->keys.back()) {
        node->right = insertRecursive(node->right, key);
        node->right->parent = node;
    } else if (node->keys.size() < maxKeys) {
        node->insertKey(key);
    }

    return node;
}

void TTree::remove(int value) {
    root = removeRecursive(root, value);
}

TTreeNode* TTree::removeRecursive(TTreeNode* node, int key) {
    if (!node) return nullptr;

    if (key < node->keys.front()) {
        node->left = removeRecursive(node->left, key);
    } else if (key > node->keys.back()) {
        node->right = removeRecursive(node->right, key);
    } else {
        node->removeKey(key);
        if (node->keys.empty()) {
            delete node;
            return nullptr;
        }
    }
    return node;
}

bool TTree::search(int value) const {
    return containsRecursive(root, value);
}

bool TTree::containsRecursive(TTreeNode* node, int key) const {
    if (!node) return false;
    if (node->containsKey(key)) return true;
    return (key < node->keys.front())
        ? containsRecursive(node->left, key)
        : containsRecursive(node->right, key);
}

void TTree::fillRandom(int size) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, size * 10);

    for (int i = 0; i < size; ++i) {
        insert(dist(gen));
    }
}

void TTree::display() const {
    displayRecursive(root, 0);
}

void TTree::displayRecursive(const TTreeNode* node, int depth) const {
    if (!node) return;
    displayRecursive(node->right, depth + 1);
    std::cout << std::setw(4 * depth) << "";
    for (int key : node->keys) std::cout << key << " ";
    std::cout << "\n";
    displayRecursive(node->left, depth + 1);
}

void TTree::clear() {
    clearRecursive(root);
    root = nullptr;
}

void TTree::clearRecursive(TTreeNode* node) {
    if (!node) return;
    clearRecursive(node->left);
    clearRecursive(node->right);
    delete node;
}