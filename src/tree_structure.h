#ifndef TREE_STRUCTURE_H
#define TREE_STRUCTURE_H

#include <iostream>

class TreeStructure {
public:
    // destruktor
    virtual ~TreeStructure() {}

        // PODSTAWOWE METODY
    virtual void insert(int key, int value) = 0;
    virtual void remove(int key) = 0;
    virtual bool search(int key, int& value) = 0;
    virtual void fillRandom(int size) = 0;
    virtual void clear() = 0;
    
    // METODY POMOCNICZE
    // wyświetlanie drzewa
    virtual void display() const = 0;
    virtual void displayStructure() const = 0;
};

#endif