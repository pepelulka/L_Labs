#include <iostream>

#include <unistd.h>
#include <dlfcn.h>

extern "C" {
    #include "lib.h"
}

void RunInterface(const char* lib1Path, const char* lib2Path) {
    int command;
    void* dls[2];
    dls[0] = dlopen(lib1Path, RTLD_LOCAL | RTLD_LAZY);
    dls[1] = dlopen(lib2Path, RTLD_LOCAL | RTLD_LAZY);
    if (dls[0] == nullptr) {
        throw std::invalid_argument("Can't open first lib.");
    }
    if (dls[1] == nullptr) {
        throw std::invalid_argument("Can't open second lib.");
    }
    size_t currentDl = 0;

    float(*func1)(float, float, float) = (float(*)(float, float, float))dlsym(dls[currentDl], "SinIntegral");
    float(*func2)(float, float) = (float(*)(float, float))dlsym(dls[currentDl], "Derivative");

    while (std::cin) {
        std::cin >> command;
        if (command == 0) {
            currentDl = 1 - currentDl;
            func1 = (float(*)(float, float, float))dlsym(dls[currentDl], "SinIntegral");
            func2 = (float(*)(float, float))dlsym(dls[currentDl], "Derivative");
        } else if (command == 1) {
            float a, b, e;
            std::cin >> a >> b >> e;
            std::cout << func1(a, b, e) << std::endl;
        } else if (command == 2) {
            float a, deltaX;
            std::cin >> a >> deltaX;
            std::cout << func2(a, deltaX) << std::endl;
        }
    }
}

int main() {
    if (getenv("PATH_TO_LIB1") == nullptr) {
        std::cout << "Define PATH_TO_LIB1\n";
        exit(EXIT_FAILURE);
    }
    if (getenv("PATH_TO_LIB2") == nullptr) {
        std::cout << "Define PATH_TO_LIB2\n";
        exit(EXIT_FAILURE);
    }
    RunInterface(getenv("PATH_TO_LIB1"), getenv("PATH_TO_LIB2"));
}
