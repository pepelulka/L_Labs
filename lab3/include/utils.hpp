#pragma once

#include <random>

constexpr int SHM_SIZE = 4096;
inline const char* SHM1_NAME = "lab3-shm-1";
inline const char* SHM2_NAME = "lab3-shm-2";

bool Probability(size_t chance);