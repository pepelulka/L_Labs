#include "lib.h"

#include <math.h>

float SinIntegral(float a, float b, float e) {
    if (a > b) {
        return -SinIntegral(b, a, e);
    }
    float sum = 0;
    while ((a + e) < b) {
        sum += e * (0.5 * sin(a) + 0.5 * sin(a + e));
        a += e;
    }
    return sum;
}

float Derivative(float a, float deltaX) {
    return (cos(a + deltaX) - cos(a - deltaX)) / (2 * deltaX);
}
