#pragma once

#include <iostream>
#include <vector>

#include <unistd.h>
#include <pthread.h>

namespace lab2 {

using ll = int64_t;
using Row = std::vector<ll>;
using Matrix = std::vector<Row>;

using SizeType = std::pair<size_t, size_t>;

// Using Samuelson–Berkowitz algorithm,
// ref: https://en.wikipedia.org/wiki/Samuelson%E2%80%93Berkowitz_algorithm
ll Det(const Matrix &a);
ll ParallelDet(const Matrix &matrix, size_t threadNum);

}
