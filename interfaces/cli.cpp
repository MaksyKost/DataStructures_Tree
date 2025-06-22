#include "cli.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <windows.h>

void CLI::setTextColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void CLI::fancyMenuTitle() {
    const std::string lines[] = {
        " __  __ _____ _   _ _   _ ",
        "|  \\/  | ____| \\ | | | | |",
        "| |\\/| |  _| |  \\| | | | |",
        "| |  | | |___| |\\  | \\_/ |",
        "|_|  |_|_____|_| \\_|\\___/ "
    };

    std::srand(static_cast<unsigned int>(std::time(0)));
    system("cls");
    for (const auto& line : lines) {
        for (char ch : line) {
            setTextColor(colors[std::rand() % 7]);
            std::cout << ch << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        std::cout << '\n';
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    for (int i = 0; i < 5; ++i) {
        setTextColor(colors[i % 7]);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "\r";
        for (const auto& line : lines) {
            std::cout << line << '\n';
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "\r";
    }

    setTextColor(0x07);
}

void CLI::menuAVLTree() {
    AVLTree tree;
    int choice, key, value;
    do {
        std::cout << "\n[ AVL TREE MENU ]\n";
        std::cout << "1. Wstaw (klucz, wartość)\n";
        std::cout << "2. Usuń po kluczu\n";
        std::cout << "3. Szukaj po kluczu\n";
        std::cout << "4. Losowe dane\n";
        std::cout << "5. Wyświetl dane\n";
        std::cout << "6. Struktura drzewa\n";
        std::cout << "7. Wyczyść drzewo\n";
        std::cout << "0. Powrót\n";
        std::cout << "Twój wybór: "; std::cin >> choice;

        switch (choice) {
            case 1: std::cin >> key >> value; tree.insert(key, value); break;
            case 2: std::cin >> key; tree.remove(key); break;
            case 3: std::cin >> key;
                if (tree.search(key, value)) std::cout << "Znaleziono: " << value << "\n";
                else std::cout << "Nie znaleziono.\n"; break;
            case 4: std::cin >> key; tree.fillRandom(key); break;
            case 5: tree.display(); break;
            case 6: tree.displayStructure(); break;
            case 7: tree.clear(); break;
        }
    } while (choice != 0);
}

void CLI::menuSplayTree() {
    SplayTree tree;
    int choice, key, value;
    do {
        std::cout << "\n[ SPLAY TREE MENU ]\n";
        std::cout << "1. Wstaw (klucz, wartość)\n";
        std::cout << "2. Usuń po kluczu\n";
        std::cout << "3. Szukaj po kluczu\n";
        std::cout << "4. Losowe dane\n";
        std::cout << "5. Wyświetl dane\n";
        std::cout << "6. Struktura drzewa\n";
        std::cout << "7. Wyczyść drzewo\n";
        std::cout << "0. Powrót\n";
        std::cout << "Twój wybór: "; std::cin >> choice;

        switch (choice) {
            case 1: std::cin >> key >> value; tree.insert(key, value); break;
            case 2: std::cin >> key; tree.remove(key); break;
            case 3: std::cin >> key;
                if (tree.search(key, value)) std::cout << "Znaleziono: " << value << "\n";
                else std::cout << "Nie znaleziono.\n"; break;
            case 4: std::cin >> key; tree.fillRandom(key); break;
            case 5: tree.display(); break;
            case 6: tree.displayStructure(); break;
            case 7: tree.clear(); break;
        }
    } while (choice != 0);
}

void CLI::menuTTree() {
    TTree tree;
    int choice, key, value;
    do {
        std::cout << "\n[ T-TREE MENU ]\n";
        std::cout << "1. Wstaw (klucz, wartość)\n";
        std::cout << "2. Usuń po kluczu\n";
        std::cout << "3. Szukaj po kluczu\n";
        std::cout << "4. Losowe dane\n";
        std::cout << "5. Wyświetl dane\n";
        std::cout << "6. Struktura drzewa\n";
        std::cout << "7. Wyczyść drzewo\n";
        std::cout << "0. Powrót\n";
        std::cout << "Twój wybór: "; std::cin >> choice;

        switch (choice) {
            case 1: std::cin >> key >> value; tree.insert(key, value); break;
            case 2: std::cin >> key; tree.remove(key); break;
            case 3: std::cin >> key;
                if (tree.search(key, value)) std::cout << "Znaleziono: " << value << "\n";
                else std::cout << "Nie znaleziono.\n"; break;
            case 4: std::cin >> key; tree.fillRandom(key); break;
            case 5: tree.display(); break;
            case 6: tree.displayStructure(); break;
            case 7: tree.clear(); break;
        }
    } while (choice != 0);
}

void CLI::menuDancingTree() {
    DancingTree tree;
    int choice, key, value;
    do {
        std::cout << "\n[ DANCING TREE MENU ]\n";
        std::cout << "1. Wstaw (klucz, wartość)\n";
        std::cout << "2. Usuń po kluczu\n";
        std::cout << "3. Szukaj po kluczu\n";
        std::cout << "4. Losowe dane\n";
        std::cout << "5. Wyświetl dane\n";
        std::cout << "6. Struktura drzewa\n";
        std::cout << "7. Wyczyść drzewo\n";
        std::cout << "0. Powrót\n";
        std::cout << "Twój wybór: "; std::cin >> choice;

        switch (choice) {
            case 1: std::cin >> key >> value; tree.insert(key, value); break;
            case 2: std::cin >> key; tree.remove(key); break;
            case 3: std::cin >> key;
                if (tree.search(key, value)) std::cout << "Znaleziono: " << value << "\n";
                else std::cout << "Nie znaleziono.\n"; break;
            case 4: std::cin >> key; tree.fillRandom(key); break;
            case 5: tree.display(); break;
            case 6: tree.displayStructure(); break;
            case 7: tree.clear(); break;
        }
    } while (choice != 0);
}

void CLI::run() {
    srand(seed);
    system("chcp 65001 > nul");
    int choice;

    do {
        fancyMenuTitle();
        std::cout << "----------------  Wybierz DRZEWO ---------------\n";
        std::cout << "| 1. AVL Tree                                   |\n";
        std::cout << "| 2. Splay Tree                                 |\n";
        std::cout << "| 3. T-Tree                                     |\n";
        std::cout << "| 4. Dancing Tree                               |\n";
        std::cout << "| 0. Wyjdź                                      |\n";
        std::cout << "Twój wybór: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                menuAVLTree();
                break;
            case 2:
                menuSplayTree();
                break;
            case 3:
                menuTTree();
                break;
            case 4:
                menuDancingTree();
                break;
        }
    } while (choice != 0);
}