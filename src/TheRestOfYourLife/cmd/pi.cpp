#include "rtweekend.h"

#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>

int main() {
    int sqrt_N = 10000;
    int incide_circle = 0;
    int incide_circle_stratified = 0;

    for (int i = 0; i < sqrt_N; ++i)
        for (int j = 0; j < sqrt_N; ++j) {
            auto x = random_double(-1, 1);
            auto y = random_double(-1, 1);
            if (x * x + y * y < 1)
                ++incide_circle;
            x = 2 * ((i + random_double()) / sqrt_N) - 1;
            y = 2 * ((j + random_double()) / sqrt_N) - 1;
            if (x * x + y * y < 1)
                ++incide_circle_stratified;
        }

    std::cout << std::fixed << std::setprecision(12);

    std::cout << "Regular    Estimate of Pi = " << (4.0 * incide_circle) / (sqrt_N * sqrt_N) << '\n';
    std::cout << "Stratified Estimate of Pi = " << (4.0 * incide_circle_stratified) / (sqrt_N * sqrt_N) << '\n';
}