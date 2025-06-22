#ifndef TTREE_H
#define TTREE_H

#include "tree_structure.h"
#include <vector>
#include <random>

class TTreeNode {
public:
    std::vector<int> keys;
    TTreeNode* left;
    TTreeNode* right;
    TTreeNode* parent;

    TTreeNode();
    bool insertKey(int key);
    bool containsKey(int key) const;
    bool removeKey(int key);
};

class TTree : public TreeStructure {
private:
    TTreeNode* root;
    const size_t maxKeys;

    TTreeNode* insertRecursive(TTreeNode* node, int key);
    TTreeNode* removeRecursive(TTreeNode* node, int key);
    bool containsRecursive(TTreeNode* node, int key) const;
    void clearRecursive(TTreeNode* node);
    void displayRecursive(const TTreeNode* node, int depth) const;

public:
    TTree(size_t maxKeysPerNode = 3);
    ~TTree();
    TTreeNode* getRoot() const { return root; }

    virtual void insert(int value) override;
    virtual void remove(int value) override;
    virtual bool search(int value) const override;
    virtual void fillRandom(int size) override;
    virtual void display() const override;
    virtual void clear() override;
};

#endif // TTREE_H