#include "include/Options.hpp"
#include <iostream>

Options::Options(int argc, char *argv[]){
    for (int i = 0; i < argc; i++) {
        std::string argument = argv[i];
        auto delimiter = argument.find("--");
    if (delimiter == -1) {
            std::cerr << "Invalid option: " << argument << '\n';
            continue;
        }
    }
}
