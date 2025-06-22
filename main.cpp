#include "interfaces/gui.h"
#include "interfaces/cli.h"
#include <iostream>
#include <string>


int main(int argc, char* argv[]) {

    std::string mode = "cli";
    if (argc > 1) {
        mode = argv[1];
    }

    if (mode == "gui") {
        // Uruchomienie trybu graficznego
        run();
    }
    else if (mode == "cli") {
        // Uruchomienie trybu konsolowego
        CLI cli;
        cli.run();

    }
    else {
        std::cerr << "Nieznany tryb: " << mode << ". Dostępne tryby: cli, gui." << std::endl;
        return 1;
    }
    return 0;
}