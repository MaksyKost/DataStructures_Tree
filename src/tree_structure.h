#ifndef TREE_STRUCTURE_H
#define TREE_STRUCTURE_H

#include <iostream>

class TreeStructure {
public:
    // Wirtualny destruktor zapewniający poprawne zarządzanie pamięcią
    virtual ~TreeStructure() {}

    // Wirtualne metody, które muszą zostać zaimplementowane w klasach pochodnych
    virtual void insert(int value) = 0;
    virtual void remove(int value) = 0;
    virtual bool search(int value) const = 0;
    virtual void fillRandom(int size) = 0;
    void clear();

    // Opcjonalna metoda do wyświetlania drzewa, może być zaimplementowana w podklasach
    virtual void display() const = 0;
};

#endif // TREE_STRUCTURE_H