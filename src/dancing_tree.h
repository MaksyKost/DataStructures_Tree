#ifndef DANCING_TREE_H
#define DANCING_TREE_H

#include "tree_structure.h"

struct DancingTreeNode {
    int key;
    int value;
    DancingTreeNode* left;
    DancingTreeNode* right;

    DancingTreeNode(int k, int val) : key(k), value(val), left(nullptr), right(nullptr) {}
};

class DancingTree : public TreeStructure {
private:
    DancingTreeNode* root;
    void deleteSubtree(DancingTreeNode* node);

public:
    DancingTree();
    ~DancingTree();

    virtual void insert(int key, int value) override;
    virtual void remove(int value) override;
    virtual bool search(int key, int& value) override;
    virtual void display() const override;
    virtual void displayStructure() const override;
};

#endif