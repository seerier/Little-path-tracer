#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
//#include <cstdlib>
#include <limits>
#include <memory>
#include <random>


//Usings

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

//Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

//Utility Functions

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180;
}

inline double radians_to_degrees(double radians){
    return radians * 180 / pi;
}

inline double random_double() {
    //Returns a random real in [0, 1).
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double random_double(double min, double max) {
    //Returna a random real in [min, max).
    return random_double() * (max - min) + min;
}

inline int random_int(int min, int max) {
    //Returns a random integer in [min, max].
    return static_cast<int>(random_double(min, max + 1));
}

//Common Headers

#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif