#include "interfaces/gui.h"
#include "interfaces/cli.h"
#include <iostream>
#include <string>


int main(int argc, char* argv[]) {
    std::string mode = "gui";
    if (argc > 1) {
        mode = argv[1];
    }

    if (mode == "gui") {
        // Uruchomienie trybu graficznego
        GUI gui;
        if (!gui.init()) {
            std::cerr << "Błąd inicjalizacji GUI (SDL)." << std::endl;
            std::cin.get(); // Zatrzymaj program, aby zobaczyć komunikat
            return 1;
        }
        Board board;
        gui.run(board);
        gui.close();
    }
    else if (mode == "cli") {
        // Uruchomienie trybu konsolowego

    }
    else {
        std::cerr << "Nieznany tryb: " << mode << ". Dostępne tryby: cli, gui." << std::endl;
        return 1;
    }
    return 0;
}