#include "rtweekend.h"

#include <algorithm>
#include <vector>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <stdlib.h>

struct sample {
    double x;
    double p_x;
};

bool compare_by_x(const sample &a, const sample &b) {
    return a.x < b.x;
}

int main() {
    int N = 10000;
    double sum = 0.0;

    //iterate through all of our samples
    std::vector<sample> samples;
    for (int i = 0; i < N; ++i) {
        //Get the area under the curve
        auto x = random_double(0, 2 * pi);
        auto sin_x = sin(x);
        auto p_x = exp(-x / (2 * pi)) * sin_x * sin_x;
        sum += p_x;
        //store this sample
        sample this_sample = { x, p_x };
        samples.push_back(this_sample);
    }

    //Sort the samples by x
    std::sort(samples.begin(), samples.end(), compare_by_x);

    //Find out the sample at which we have half of our area
    double half_sum = sum / 2.0;
    double halfway_point = 0.0;
    double accum = 0.0;
    for (int i = 0; i < N; ++i) {
        accum += samples[i].p_x;
        if (accum >= half_sum) {
            halfway_point = samples[i].x;
            break;
        }
    }

    std::cout << std::fixed << std::setprecision(12);
    std::cout << "Average = " << sum / N << '\n';
    std::cout << "Area under curve = " << 2 * pi * sum / N << '\n';
    std::cout << "Halfway = " << halfway_point << '\n';
}