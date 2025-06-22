#include <iostream>
#include <functional>
#include "avl_tree.h"

AVLTree::AVLTree() {
    root = nullptr;
}

void AVLTree::clear(AVLNode* node) {
    if (node == nullptr) return;
    clear(node->left);
    clear(node->right);
    delete node;
}

void AVLTree::clear() {
    clear(root);
    root = nullptr;
}

int AVLTree::height(AVLNode *node) {
    return node ? node->height : 0;
}

int AVLTree::balanceFactor(AVLNode *node) {
    return height(node->left) - height(node->right);
}

int AVLTree::max(int a, int b) {
    return (a > b ? a : b);
}

AVLNode* AVLTree::rotateLeft(AVLNode* node) {
    AVLNode* y = node->right;
    node->right = y->left;
    y->left = node;
    node->height = max(height(node->left), height(node->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    return y;
}

AVLNode* AVLTree::rotateRight(AVLNode *node) {
    AVLNode *y = node->left;
    node->left = y->right;
    y->right = node;
    node->height = max(height(node->left), height(node->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    return y;
}

AVLNode* AVLTree::balance(AVLNode* node) {
    //LL
    if (balanceFactor(node) > 1 && balanceFactor(node->left) >= 0) node = rotateRight(node);
    //LR
    else if (balanceFactor(node) > 1 && balanceFactor(node->left) < 0) {
        node->left = rotateLeft(node->left);
        node = rotateRight(node);
    }
    //RR
    else if (balanceFactor(node) < -1 && balanceFactor(node->right) <= 0) node = rotateLeft(node);
    //RL
    else if (balanceFactor(node) < -1 && balanceFactor(node->right) > 0) {
        node->right = rotateRight(node->right);
        node = rotateLeft(node);
    }
    return node;
}

AVLNode* AVLTree::insert(AVLNode* node, int key, int value) {
    //
    if (node == nullptr) 
        return new AVLNode(key, value);
    //
    if (key < node->key) 
        node->left = insert(node->left, key, value);
    //
    else if (key > node->key) 
        node->right = insert(node->right, key, value);
    //
    else {
        node->value = value;
        return node;
    }

    node->height = 1 + max(height(node->left), height(node->right));
    node = balance(node);
    return node;
}

void AVLTree::insert(int key, int value) {
    root = insert(root, key, value);
}

bool AVLTree::find_helper(AVLNode* node, int key, int& value) const {
    if (node == nullptr) {
        return false;
    }
    if (key == node->key) {
        value = node->value;
        return true;
    }
    else if (key < node->key)
        return find_helper(node->left, key, value);
    else
        return find_helper(node->right, key, value);
}

bool AVLTree::search(int key, int& value) {
    return find_helper(root, key, value);
}

//
AVLNode* AVLTree::minValueNode(AVLNode* node) {
    while (node->left != nullptr)
        node = node->left;
    return node;
}

AVLNode* AVLTree::remove(AVLNode *node, int key) {
    if (node == nullptr) {
        //std::cerr << "Node with this key was not found" << std::endl;
        return node;
    }
    //
    if (key < node->key) 
        node->left = remove(node->left, key);
    else if (key > node->key)
        node->right = remove(node->right, key);
    
    else {
        //
        if (!node->left && !node->right) {
            delete node;
            return nullptr;
        }
        else if (!node->left) {
            AVLNode* temp = node->right;
            delete node;
            return temp;
        }
        else if (!node->right) {
            AVLNode* temp = node->left;
            delete node;
            return temp;
        }
        else {
            AVLNode* minNode = minValueNode(node->right);
            node->key = minNode->key;
            node->value = minNode->value;
            node->right = remove(node->right, minNode->key);
        }

    }

    node->height = 1 + max(height(node->left), height(node->right));
    node = balance(node);
    return node;
}

void AVLTree::remove(int key) {
    root = remove(root, key);
}

AVLTree::~AVLTree() {
    clear();
}

void printInOrderAVL(const AVLNode* node) {
    if (!node) return;
    printInOrderAVL(node->left);
    std::cout << "(" << node->key << ":" << node->value << ") ";
    printInOrderAVL(node->right);
}

void AVLTree::display() const {
    printInOrderAVL(root);
    std::cout << std::endl;
}

void printPreOrderAVL(const AVLNode* node) {
    if (!node) return;
    std::cout << "(" << node->key << ":" << node->value << ", H:" << node->height << ") ";
    printPreOrderAVL(node->left);
    printPreOrderAVL(node->right);
}

void AVLTree::displayStructure() const {
    std::cout << "Pre-order traversal (Root, Left, Right): ";
    printPreOrderAVL(root);
    std::cout << std::endl;
}