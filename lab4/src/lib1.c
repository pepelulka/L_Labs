#include "lib.h"

#include <math.h>

float SinIntegral(float a, float b, float e) {
    if (a > b) {
        return -SinIntegral(b, a, e);
    }
    float sum = 0;
    while ((a + e) < b) {
        sum += e * sin(a + 0.5 * e);
        a += e;
    }
    return sum;
}

float Derivative(float a, float deltaX) {
    return (cos(a + deltaX) - cos(a)) / deltaX;
}
