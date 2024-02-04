#include <chrono>
#include <random>

#include <gtest/gtest.h>

#include "det.hpp"

using namespace lab2;
using namespace std::chrono;

Matrix GenerateRandomSquareMatrix(size_t n) {
    Matrix a(n, Row(n, 0));
    std::mt19937 rnd(time(nullptr));
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            a[i][j] = rnd() % 3 - 1;
        }
    }
    return a;
}

Matrix GenerateIdentity(size_t n) {
    Matrix a(n, Row(n, 0));
    for (size_t i = 0; i < n; i++) {
        a[i][i] = 1;
    }
    return a;
}

TEST(Lab2, TestIdentityNonParallel) {
    Matrix a = GenerateIdentity(5);
    EXPECT_EQ(Det(a), 1);
    a = GenerateIdentity(15);
    EXPECT_EQ(Det(a), 1);
    a = GenerateIdentity(25);
    EXPECT_EQ(Det(a), 1);
}

TEST(Lab2, TestIdentityParallel) {
    Matrix a = GenerateIdentity(5);
    EXPECT_EQ(ParallelDet(a, 4), 1);
    a = GenerateIdentity(15);
    EXPECT_EQ(ParallelDet(a, 8), 1);
    a = GenerateIdentity(25);
    EXPECT_EQ(ParallelDet(a, 12), 1);
}

TEST(Lab2, TestRandomNonParallel) {
    Matrix a = {
        {4, 1, 8, 2, 4, 9},
        {7, 5, 4, 9, 4, 6},
        {1, 0, 1, 1, 7, 2},
        {9, 0, 9, 5, 4, 5},
        {6, 2, 8, 5, 7, 3},
        {3, 2, 8, 7, 2, 9}
    };
    ll expected = -42256;

    EXPECT_EQ(Det(a), expected);
}

TEST(Lab2, TestRandomParallel) {
    Matrix a = {
        {4, 1, 8, 2, 4, 9},
        {7, 5, 4, 9, 4, 6},
        {1, 0, 1, 1, 7, 2},
        {9, 0, 9, 5, 4, 5},
        {6, 2, 8, 5, 7, 3},
        {3, 2, 8, 7, 2, 9}
    };
    ll expected = -42256;

    EXPECT_EQ(ParallelDet(a, 5), expected);
}

TEST(Lab2, SpeedTest) {
    Matrix a = GenerateRandomSquareMatrix(25);
    ll result, resultParallel;
    size_t duration, durationParallel;

    auto start = high_resolution_clock::now();
    result = Det(a);
    auto stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start).count();

    start = high_resolution_clock::now();
    resultParallel = ParallelDet(a, 8);
    stop = high_resolution_clock::now();
    durationParallel = duration_cast<microseconds>(stop - start).count();

    EXPECT_EQ(result, resultParallel);
    EXPECT_LT(durationParallel, duration);
}
