#include "rtweekend.h"

#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdio>

double f(const vec3 &d) {
    auto cosine_squared = d.z() * d.z();
    return cosine_squared;
}

double pdf(const vec3 &d) {
    return 1 / (4 * pi);
}



int main() {
    int N = 1000000;
    auto sum = 0.0;
    for (int i = 0; i < N; ++i) {
        vec3 vec = random_unit_vector();
        sum += f(vec) / pdf(vec);
    }
    std::cout << std::fixed << std::setprecision(12);
    std::cout << "I = " << sum / N << '\n';
}