#include "t_tree.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

TTree::TTree() : TTree(3) {}
TTree::TTree(size_t maxKeysPerNode)
    : root(nullptr), maxKeys(maxKeysPerNode) {}

bool TTreeNode::insertKey(int key, int value) {
    auto it = std::find(keys.begin(), keys.end(), key);
    if (it != keys.end())
        return false;
    keys.push_back(key);
    values.push_back(value);
    for (size_t i = keys.size() - 1; i > 0 && keys[i] < keys[i - 1]; --i) {
        std::swap(keys[i], keys[i - 1]);
        std::swap(values[i], values[i - 1]);
    }
    return true;
}

bool TTreeNode::containsKey(int key) const {
    return std::find(keys.begin(), keys.end(), key) != keys.end();
}

bool TTreeNode::removeKey(int key) {
    auto it = std::find(keys.begin(), keys.end(), key);
    if (it != keys.end()) {
        size_t idx = std::distance(keys.begin(), it);
        keys.erase(it);
        values.erase(values.begin() + idx);
        return true;
    }
    return false;
}

int TTreeNode::getValue(int key) const {
    auto it = std::find(keys.begin(), keys.end(), key);
    if (it != keys.end()) {
        size_t idx = std::distance(keys.begin(), it);
        return values[idx];
    }
    return 0;
}

TTree::~TTree() {
    clear();
}

void TTree::insert(int key, int value) {
    root = insertRecursive(root, key, value);
}

TTreeNode* TTree::insertRecursive(TTreeNode* node, int key, int value) {
    if (!node) {
        node = new TTreeNode();
        node->insertKey(key, value);
        return node;
    }

    // Если ключ уже есть в этом узле — не вставляем дубликаты
    if (node->containsKey(key))
        return node;

    // Если есть место и ключ попадает в диапазон — вставляем в этот узел
    if (node->keys.size() < maxKeys &&
        (key > node->keys.front() && key < node->keys.back() || node->keys.size() == 1)) {
        node->insertKey(key, value);
        return node;
    }

    // Если ключ меньше минимального — идём влево
    if (key < node->keys.front()) {
        node->left = insertRecursive(node->left, key, value);
        if (node->left) node->left->parent = node;
    }
    // Если ключ больше максимального — идём вправо
    else if (key > node->keys.back()) {
        node->right = insertRecursive(node->right, key, value);
        if (node->right) node->right->parent = node;
    }
    // Если нет места, но ключ попадает в диапазон — для простоты вставим вправо
    else {
        node->right = insertRecursive(node->right, key, value);
        if (node->right) node->right->parent = node;
    }
    return node;
}

void TTree::remove(int key) {
    root = removeRecursive(root, key);
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

bool TTree::search(int key, int& value) {
    return searchRecursive(root, key, value);
}

bool TTree::searchRecursive(TTreeNode* node, int key, int& value) const {
    if (!node) return false;
    auto it = std::find(node->keys.begin(), node->keys.end(), key);
    if (it != node->keys.end()) {
        size_t idx = std::distance(node->keys.begin(), it);
        value = node->values[idx];
        return true;
    }
    if (key < node->keys.front())
        return searchRecursive(node->left, key, value);
    else
        return searchRecursive(node->right, key, value);
}

void TTree::fillRandom(int size) {
    clear();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, size * 10);

    for (int i = 0; i < size; ++i) {
        int key = dist(gen);
        int value = dist(gen);
        insert(key, value);
    }
}

void TTree::display() const {
    displayRecursive(root, 0);
}

void TTree::displayRecursive(const TTreeNode* node, int depth) const {
    if (!node) return;
    displayRecursive(node->right, depth + 1);
    std::cout << std::setw(4 * depth) << "";
    for (size_t i = 0; i < node->keys.size(); ++i)
        std::cout << node->keys[i] << ":" << node->values[i] << " ";
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