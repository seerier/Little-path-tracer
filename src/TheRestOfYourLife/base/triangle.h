#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "quad.h"

class triangle : public quad {
public:
    triangle(const point3 &_Q, const vec3 &_u, const vec3 &_v, shared_ptr<material> m)
        :quad(_Q, _u, _v, m) {}
    
    bool is_interior(double a, double b, hit_record &rec) const override {
        //Given the hit point in plane coordinates, return false if it is outside the
        //primitive, otherwise set the hit record UV coordinates and return true;

        if ((a < 0) || (b < 0) || (a + b > 1))
            return false;

        rec.u = a;
        rec.v = b;
        return true;
    }
};

#endif