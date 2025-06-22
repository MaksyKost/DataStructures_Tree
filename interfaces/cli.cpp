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

void CLI::menuHeap(HeapPriorityQueue& heap) {
    int choice, e, p;
    do {
        std::cout << "\n|------------------------- Heap PQ MENU ------------------------|\n";
        std::cout << "| 1. Dodanie elementu e o priorytecie p                           |\n";
        std::cout << "| 2. Usunięcie i zwrócenie elementu o największym priorytecie     |\n";
        std::cout << "| 3. Zwrócenie elementu o największym priorytecie                 |\n";
        std::cout << "| 4. Zmiana priorytetu elementu e na p                            |\n";
        std::cout << "| 5. Zwrócenie rozmiaru                                           |\n";
        std::cout << "| 6. Wypełnianie losowymi elementami                              |\n";
        std::cout << "| 7. Wyświetlanie kolejki                                         |\n";
        std::cout << "| 8. Czyszczenie kolejki                                          |\n";
        std::cout << "| 0. Wyjdź                                                        |\n";
        std::cout << "|-----------------------------------------------------------------|\n";
        std::cout << "Twój wybór: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                std::cout << "Podaj wartość: "; std::cin >> e;
                std::cout << "Podaj priorytet: "; std::cin >> p;
                heap.insert(e, p);
                break;
            case 2: {
                Element max = heap.extractMax();
                std::cout << "Usunięto: (" << max.e << ", " << max.p << ")\n";
                break;
            }
            case 3: {
                Element top = heap.peek();
                std::cout << "Najwyższy priorytet: (" << top.e << ", " << top.p << ")\n";
                break;
            }
            case 4:
                std::cout << "Podaj wartość: "; std::cin >> e;
                std::cout << "Podaj nowy priorytet: "; std::cin >> p;
                heap.modifyKey(e, p);
                break;
            case 5:
                std::cout << "Rozmiar: " << heap.returnSize() << "\n";
                break;
            case 6:
                std::cout << "Ile losowych?: "; std::cin >> e;
                heap.fillRandom(e);
                break;
            case 7:
                heap.print();
                break;
            case 8:
                heap.clear();
                break;
        }
    } while (choice != 0);
}

void CLI::menuDDL(PriorityQueueDLL& ddl) {
    int choice, e, p;
    do {
        std::cout << "\n|------------------ DLL PQ MENU ------------------|\n";
        std::cout << "| 1. Dodaj (e,p)                                   |\n";
        std::cout << "| 2. Usuń max                                      |\n";
        std::cout << "| 3. Podejrzyj max                                 |\n";
        std::cout << "| 4. Zmień priorytet                               |\n";
        std::cout << "| 5. Rozmiar                                       |\n";
        std::cout << "| 6. Losowe dane                                   |\n";
        std::cout << "| 7. Wyświetl                                      |\n";
        std::cout << "| 8. Wyczyść                                       |\n";
        std::cout << "| 0. Wyjdź                                         |\n";
        std::cout << "|--------------------------------------------------|\n";
        std::cout << "Twój wybór: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                std::cout << "Podaj wartość: "; std::cin >> e;
                std::cout << "Podaj priorytet: "; std::cin >> p;
                ddl.insert(e, p);
                break;
            case 2:
                std::cout << "Usunięto: " << ddl.extractMax() << "\n";
                break;
            case 3:
                std::cout << "Najwyższy priorytet ma: " << ddl.peek() << "\n";
                break;
            case 4:
                std::cout << "Wartość elementu: "; std::cin >> e;
                std::cout << "Nowy priorytet: "; std::cin >> p;
                ddl.modifyKey(e, p);
                break;
            case 5:
                std::cout << "Rozmiar: " << ddl.returnSize() << "\n";
                break;
            case 6:
                std::cout << "Ile losowych?: "; std::cin >> e;
                ddl.fillRandom(e);
                break;
            case 7:
                ddl.print();
                break;
            case 8:
                ddl.clear();
                break;
        }
    } while (choice != 0);
}

void CLI::run() {
    srand(seed);
    system("chcp 65001 > nul");
    int choice;
    do {
        fancyMenuTitle();
        std::cout << "----------------  Wybierz opcję ---------------\n";
        std::cout << "| 1. Kopiec                                    |\n";
        std::cout << "| 2. Lista dwukierunkowa                       |\n";
        std::cout << "| 0. Wyjdź                                     |\n";
        std::cout << "Twój wybór: ";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                HeapPriorityQueue heapPQ;
                menuHeap(heapPQ);
                break;
            }
            case 2: {
                PriorityQueueDLL ddlPQ;
                menuDDL(ddlPQ);
                break;
            }
        }
    } while (choice != 0);
}