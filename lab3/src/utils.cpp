#include "utils.hpp"

bool Probability(size_t chance) {
    std::mt19937 r(time(NULL));
    size_t res = r() % 100;
    return (res < chance);
}