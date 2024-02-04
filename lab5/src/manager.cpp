#include <iostream>

#include "manager_base.hpp"

using namespace lab5;

int main() {
    if (getenv("PATH_TO_COMPUTER") == nullptr) {
        std::cout << "Define PATH_TO_COMPUTER!\n";
    }
    Frontend f(getenv("PATH_TO_COMPUTER"));
    f.Routine();
}
