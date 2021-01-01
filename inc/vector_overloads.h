#pragma once

#include "ww_math.h"

using fp::vector;

inline vector operator-(const vector &vec) {
    return vector(-vec.x, -vec.y);
}

inline vector operator+(const vector &v1, const vector &v2) {
    return vector(v1.x + v2.x, v1.y + v2.y);
}

inline vector operator-(const vector &v1, const vector &v2) {
    return vector(v1.x - v2.x, v1.y - v2.y);
}

inline vector operator/(const vector &v1, const vector &v2) {
    return vector(v1.x / v2.x, v1.y / v2.y);
}

inline vector operator*(const vector &v, const fixed_point &n) {
    return vector(v.x * n, v.y * n);
}

inline vector operator/(vector v, const fixed_point &n) {
    return vector(v.x / n, v.y / n);
}

inline vector operator*(const fixed_point &n, const vector &v) {
    return v * n;
}

inline vector &operator-=(vector &v1, const vector &v2) {
    v1.x -= v2.x;
    v1.y -= v2.y;
    return v1;
}

inline vector &operator+=(vector &v1, const vector &v2) {
    v1.x += v2.x;
    v1.y += v2.y;
    return v1;
}

inline vector &operator*=(vector &v, const fixed_point &n) {
    v.x *= n;
    v.y *= n;
    return v;
}

inline vector orthogonal(const vector &v) {
    return vector(-v.y, v.x);
}
