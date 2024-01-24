// 22 вариант

#include "parent.hpp"

int main() {
    if (getenv("PATH_TO_CHILD") == nullptr) {
        std::cerr << "Define PATH_TO_CHILD!\n";
        exit(EXIT_FAILURE);
    }
    ParentRoutine(getenv("PATH_TO_CHILD"), std::cin);
}
