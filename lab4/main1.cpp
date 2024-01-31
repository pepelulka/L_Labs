#include <iostream>

extern "C" {
    #include "lib.h"
}

void RunInterface() {
    int command;
    while (std::cin) {
        std::cin >> command;
        if (command == 1) {
            float a, b, e;
            std::cin >> a >> b >> e;
            std::cout << SinIntegral(a, b, e) << std::endl;
        } else if (command == 2) {
            float a, deltaX;
            std::cin >> a >> deltaX;
            std::cout << Derivative(a, deltaX) << std::endl;
        }
    }
}

int main() {
    RunInterface();
}
