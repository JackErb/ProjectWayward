#ifndef fp_overloads_h
#define fp_overloads_h

#include "ww_math.h"

/* Operator overrides */

inline FixedPoint operator+(const FixedPoint &v1, const FixedPoint &v2) {
    return FixedPoint(v1.n + v2.n);
}

inline FixedPoint operator-(const FixedPoint &v1, const FixedPoint &v2) {
    return FixedPoint(v1.n - v2.n);
}

inline FixedPoint operator/(const FixedPoint &v1, const FixedPoint &v2) {
    if (v2.n == 0) {
        std::cerr << "ERROR DIVIDE BY 0" << std::endl;
        return FixedPoint(0);
    }
    return FixedPoint(v1.n * FP_MULT / v2.n);
}

inline FixedPoint operator*(const FixedPoint &v1, const FixedPoint &v2) {
    return FixedPoint(v1.n * v2.n / FP_MULT);
}

inline bool operator<(const FixedPoint& v1, const FixedPoint& v2) {
    return v1.n < v2.n;
}
inline bool operator>(const FixedPoint& v1, const FixedPoint& v2) {
    return v1.n > v2.n;
}

inline bool operator==(const FixedPoint& v1, const FixedPoint& v2) {
    return v1.n == v2.n;
}

inline bool operator>=(const FixedPoint& v1, const FixedPoint& v2) {
    return v1 > v2 || v1 == v2;
}

inline bool operator<=(const FixedPoint& v1, const FixedPoint& v2) {
    return v1 < v2 || v1 == v2;
}

inline FixedPoint operator-(const FixedPoint& v) {
    return FixedPoint(-v.n);
}

inline FixedPoint &operator+=(FixedPoint &v1, const FixedPoint &v2) {
    v1.n += v2.n;
    return v1;
}

inline FixedPoint &operator*=(FixedPoint &v1, const FixedPoint &v2) {
    v1.n = v1.n * v2.n / FP_MULT;
    return v1;
}

inline FixedPoint &operator-=(FixedPoint &v1, const FixedPoint &v2) {
    v1.n = v1.n - v2.n;
    return v1;
}

inline FixedPoint &operator/=(FixedPoint &v1, const FixedPoint &v2) {
    v1.n = v1.n * FP_MULT / v2.n;
    return v1;
}

#endif  /* fp_overloads_h */
