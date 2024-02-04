#include <iostream>
#include <fstream>
#include <chrono>

#include "det.hpp"

using namespace std::chrono;
using namespace lab2;

Matrix GetSquareMatrixFromFile(const std::string& filename) {
    std::ifstream in(filename);
    size_t n;
    in >> n;
    Matrix result(n, Row(n, 0));
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            in >> result[i][j];
        }
    }
    return result;
}

int main(int argc, char** argv) {
    size_t threadNum = 4;

    std::string filename;
    if (argc < 2) {
        std::cout << "Missing arguments\n";
        exit(EXIT_FAILURE);
    }
    if (argc >= 2) {
        filename = std::string(argv[1]);
    }
    if (argc >= 3) {
        threadNum = std::atoi(argv[2]);
    }
    Matrix m = GetSquareMatrixFromFile(filename);

    auto start = high_resolution_clock::now();
    ll result = ParallelDet(m, threadNum);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    std::cout << "Parallel program gets " << result << " Time: " << duration.count() << " microseconds" << std::endl;

    start = high_resolution_clock::now();
    result = Det(m);
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);

    std::cout << "Non-parallel program gets " << result << " Time: " << duration.count() << " microseconds" << std::endl;
}