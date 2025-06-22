#ifndef TTREE_H
#define TTREE_H

#include "tree_structure.h"
#include <vector>
#include <random>

class TTreeNode {
public:
    std::vector<int> keys;
    TTreeNode* left = nullptr;
    TTreeNode* right = nullptr;
    TTreeNode* parent = nullptr;
    TTreeNode() = default;

    bool insertKey(int key, int value);
    bool containsKey(int key) const;
    bool removeKey(int key);
    int getValue(int key) const;
    std::vector<int> values;
};

class TTree : public TreeStructure {
private:
    TTreeNode* root;
    const size_t maxKeys;

    TTreeNode* insertRecursive(TTreeNode* node, int key, int value);
    TTreeNode* removeRecursive(TTreeNode* node, int key);
    bool searchRecursive(TTreeNode* node, int key, int& value) const;
    void clearRecursive(TTreeNode* node);
    void displayRecursive(const TTreeNode* node, int depth) const;

public:
    TTree();
    TTree(size_t maxKeysPerNode);
    ~TTree();
    TTreeNode* getRoot() const { return root; }

    void insert(int key, int value) override;
    void remove(int key) override;
    bool search(int key, int& value) override;
    void fillRandom(int size) override;
    void display() const override;
    void displayStructure() const override {}
    void clear() override;
};

#endif // TTREE_H