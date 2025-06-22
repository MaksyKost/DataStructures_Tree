#pragma once
#include "../src/avl_tree.h"
#include "../src/dancing_tree.h"
#include "../src/splay_tree.h"
#include "../src/t_tree.h"

class CLI {
private:
    int seed = 123;
    int colors[7] = { 0x0C, 0x0A, 0x0E, 0x09, 0x0B, 0x0D, 0x08 };

    void setTextColor(int color);
    void fancyMenuTitle();
    
    void menuAVLTree();
    void menuSplayTree();
    void menuTTree();
    void menuDancingTree();
    

public:
    void run();
};