#include <iostream>

#include "computer_base.hpp"

using namespace lab5;

int main(int argc, char ** argv) {
    if (argc < 4) {
        std::cout << "Missing arguments\n";
        exit(EXIT_FAILURE);
    }
    size_t id = std::atoi(argv[1]), port = std::atoi(argv[2]), parentPort = std::atoi(argv[3]);
    Computer comp(id, port, parentPort, argv[0]);
    comp.Routine();
}
