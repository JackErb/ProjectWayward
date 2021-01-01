#pragma once

#include "ww_math.h"
#include <iostream>

using fp::fixed_point;

/* Operator overrides */

inline fixed_point operator+(fixed_point v1, fixed_point v2) {
    return fixed_point(v1.n + v2.n);
}

inline fixed_point operator-(fixed_point v1, fixed_point v2) {
    return fixed_point(v1.n - v2.n);
}

inline fixed_point operator/(fixed_point v1, fixed_point v2) {
    if (v2.n == 0) {
        std::cerr << "ERROR DIVIDE BY 0" << std::endl;
        return fixed_point(0);
    }
    return fixed_point(v1.n * FP_MULT / v2.n);
}

inline fixed_point operator*(fixed_point v1, fixed_point v2) {
    return fixed_point(v1.n * v2.n / FP_MULT);
}

inline bool operator<(fixed_point v1, fixed_point v2) {
    return v1.n < v2.n;
}
inline bool operator>(fixed_point v1, fixed_point v2) {
    return v1.n > v2.n;
}

inline bool operator==(fixed_point v1, fixed_point v2) {
    return v1.n == v2.n;
}

inline bool operator>=(fixed_point v1, fixed_point v2) {
    return v1 > v2 || v1 == v2;
}

inline bool operator<=(fixed_point v1, fixed_point v2) {
    return v1 < v2 || v1 == v2;
}

inline fixed_point operator-(fixed_point v) {
    return fixed_point(-v.n);
}

inline fixed_point& operator+=(fixed_point& v1, fixed_point v2) {
    v1.n += v2.n;
    return v1;
}

inline fixed_point& operator*=(fixed_point& v1, fixed_point v2) {
    v1.n = v1.n * v2.n / FP_MULT;
    return v1;
}

inline fixed_point& operator-=(fixed_point& v1, fixed_point v2) {
    v1.n = v1.n - v2.n;
    return v1;
}

inline fixed_point& operator/=(fixed_point& v1, fixed_point v2) {
    v1.n = v1.n * FP_MULT / v2.n;
    return v1;
}
