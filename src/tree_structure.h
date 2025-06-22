#ifndef TREE_STRUCTURE_H
#define TREE_STRUCTURE_H

#include <iostream>

class TreeStructure {
public:
    // destruktor
    virtual ~TreeStructure() {}

    // PODSTAWOWE METODY
    virtual void insert(int value) = 0;
    virtual void remove(int value) = 0;
    virtual bool search(int value) const = 0;

    // METODY POMOCNICZE
    // wyświetlanie drzewa
    virtual void display() const = 0;
};

#endif