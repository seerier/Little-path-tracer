#ifndef INTERVAL_H
#define INTERVAL_H

class interval {
public:
    double min, max;

    interval() :min(+infinity), max(-infinity) {}

    interval(double _min, double _max) :min(_min), max(_max) {}

    interval(const interval &a, const interval &b)
        :min(fmin(a.min, b.min)), max(fmax(a.max, b.max)) {}
    

    bool contains(double x) const {
        return min <= x && x <= max;
    }

    bool surrounds(double x) const {
        return min < x && x < max;
    }

    double clamp(double x) const {
        if (x < min)return min;
        if (x > max)return max;
        return x;
    }

    double size() const {
        return max - min;
    }

    interval expand(double delta) const {
        auto padding = delta / 2;
        return interval(min - padding, max + padding);
    }

    static const interval empty, universe;
};

const interval interval::empty(+infinity, -infinity);
const interval interval::universe(-infinity, +infinity);

interval operator+(const interval &ival, double displacement) {
    return interval(ival.min + displacement, ival.max + displacement);
}

interval operator+(double displacement, const interval &ival) {
    return ival + displacement;
}

inline interval operator*(const interval &ival, double inv_scaling) {
    return interval(ival.min * inv_scaling, ival.max * inv_scaling);
}

inline interval operator*(double inv_scaling, const interval &ival) {
    return ival * inv_scaling;
}

#endif