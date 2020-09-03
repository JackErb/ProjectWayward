#ifndef fp_overloads_h
#define fp_overloads_h



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
    long long rn = v1.n * FixedPoint::MULT / v2.n;
    return FixedPoint(rn);
}

inline FixedPoint operator*(const FixedPoint &v1, const FixedPoint &v2) {
    long long rn = v1.n * v2.n / FixedPoint::MULT;
    return FixedPoint(rn);
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


#endif  /* fp_overloads_h */
