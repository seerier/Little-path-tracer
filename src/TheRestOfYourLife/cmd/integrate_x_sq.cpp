#include "rtweekend.h"

#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdio>

double f(double d) {
    return 8.0 * pow(d, (1.0 / 3.0));
}

double pdf(double x) {
    return (3.0 / 8.0) * x * x;
}

int main() {
    int N = 1000000;
    auto sum = 0.0;
    for (int i = 0; i < N; ++i) {
        auto x = f(random_double());
        sum += x * x / pdf(x);
    }
    std::cout << std::fixed << std::setprecision(12);
    std::cout << "I = " << sum / N << '\n';
}