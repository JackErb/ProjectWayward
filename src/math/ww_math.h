#ifndef WWMath_h
#define WWMath_h

#include <vector>
#include <iostream>

struct FixedPoint {
  public:
  	FixedPoint() : n(0) {}
	FixedPoint(long long n) : n(n) {}

	long long n;
	float toFloat();

	/* Static members */
	static FixedPoint MIN;
	static FixedPoint MAX;
	static FixedPoint PI;

	static int BASE;
	static int MULT;
	static FixedPoint fromInt(int n);
	static FixedPoint fromFloat(float n);
};

struct Vector2D {
	FixedPoint x;
	FixedPoint y;
};

typedef std::vector<Vector2D> Polygon;

namespace WWMath {
	Vector2D geometricCenter(const Polygon &p);
}






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


#endif  /* WWMath_h */
