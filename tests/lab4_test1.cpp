#include <gtest/gtest.h>

#include <iostream>
#include <algorithm>

extern "C" {
    #include "lib.h"
}

constexpr const float EPS = 1e-1;
constexpr const float PI = 3.14159265359;

bool EqualFloat(float lhs, float rhs) {
    return std::abs(lhs - rhs) < EPS;
}

TEST(Lab4, DerivativeTestLib1) {
    EXPECT_TRUE(EqualFloat(Derivative(PI, EPS), 0));
}

TEST(Lab4, SinIntegralTestLib1) {
    EXPECT_TRUE(EqualFloat(SinIntegral(0, PI, 1e-6), 2));
}