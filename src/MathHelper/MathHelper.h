//
//  MathHelper.h
//  SFML-App
//
//  Created by Jack Erb on 4/28/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef MathHelper_hpp
#define MathHelper_hpp

#include <vector>
#include <iostream>

typedef struct FixedPoint {
	static FixedPoint PI;
	static FixedPoint MAX;
	static FixedPoint MIN;
	static FixedPoint ZERO;
    static int BASE;
	static int MULT;

	static FixedPoint FromFloat(float f);
    static FixedPoint FromInt(int n);

	FixedPoint() : n(0), sign(false) {}
    FixedPoint(int fpint, int fpdec, bool s = false);
    FixedPoint(int n, bool s = false) : n(n), sign(s) {}
    
    long long n;
    bool sign;
    
    // Get the float representation
    // DO NOT USE THIS FOR GAMEPLAY PURPOSES! Only things that can
    // be non deterministic, i.e. graphics
	float f() const;
	int i() const;
} fpoat;

typedef struct Vector {
	Vector() : x(0), y(0) {}
	Vector(const fpoat &x, const fpoat &y) : x(x), y(y) {}

	fpoat x, y;
} VectorV;

typedef std::vector<VectorV> PolygonV;


fpoat atan2(const fpoat &y, const fpoat &x);

fpoat fpmin(const fpoat &v1, const fpoat &v2);
fpoat fpmax(const fpoat &v1, const fpoat &v2);
fpoat fpabs(const fpoat &v);

fpoat fpsin(const fpoat& v);
fpoat fpcos(const fpoat& v);
fpoat fpatan2(const fpoat& y, const fpoat& x);
fpoat fpsqrt(const fpoat& v);

VectorV unit_vec(VectorV v);

fpoat dot(const VectorV &v1, const VectorV &v2);

VectorV geometric_center(const PolygonV &p, int dir = 1);

inline fpoat operator+(const fpoat &v1, const fpoat &v2) {
    long long rn = (v1.sign ? -v1.n : v1.n) + (v2.sign ? -v2.n : v2.n);
    bool sign = rn < 0;
    return fpoat(abs(rn), sign);
}

inline fpoat operator-(const fpoat &v1, const fpoat &v2) {
    long long rn = (v1.sign ? -v1.n : v1.n) - (v2.sign ? -v2.n : v2.n);
    bool sign = rn < 0;
    return fpoat(abs(rn), sign);
}

inline fpoat operator/(const fpoat &v1, const fpoat &v2) {
    if (v2.n == 0) {
        std::cerr << "ERROR DIVIDE BY 0" << std::endl;
        return fpoat(0);
    }
    long long rn = v1.n * FixedPoint::MULT / v2.n;
    return fpoat(rn, v1.sign != v2.sign);
}

inline fpoat operator*(const fpoat &v1, const fpoat &v2) {
    long long rn = v1.n * v2.n / FixedPoint::MULT;
    return fpoat(rn, v1.sign != v2.sign);
}

inline bool operator<(const fpoat& v1, const fpoat& v2) {
    long long s1 = (((long long)v1.sign) * 2 - 1) * -1;
    long long s2 = (((long long)v2.sign) * 2 - 1) * -1;
    return s1 * v1.n < s2 * v2.n;
}
inline bool operator>(const fpoat& v1, const fpoat& v2) {
    long long s1 = (((long long)v1.sign) * 2 - 1) * -1;
    long long s2 = (((long long)v2.sign) * 2 - 1) * -1;
    return s1 * v1.n > s2 * v2.n;
}

inline bool operator==(const fpoat& v1, const fpoat& v2) {
    return v1.n == v2.n && v1.sign == v2.sign;
}

inline bool operator>=(const fpoat& v1, const fpoat& v2) {
    return v1 > v2 || v1 == v2;
}

inline bool operator<=(const fpoat& v1, const fpoat& v2) {
    return v1 < v2 || v1 == v2;
}

inline fpoat operator-(const fpoat& v) {
    return fpoat(v.n, !v.sign);
}

inline VectorV operator+(const VectorV &v1, const VectorV &v2) {
    return VectorV(v1.x + v2.x, v1.y + v2.y);
}

inline VectorV operator-(const Vector &v1, const Vector &v2) {
    return VectorV(v1.x - v2.x, v1.y - v2.y);
}

inline VectorV operator/(const VectorV &v, const fpoat &f) {
    return VectorV(v.x / f, v.y / f);
}

inline VectorV operator*(const VectorV &v, const fpoat &f) {
    return VectorV(v.x * f, v.y * f);
}

inline VectorV operator+(const VectorV &v, const fpoat &f) {
    return VectorV(v.x + f, v.y + f);
}

inline VectorV operator-(const VectorV &v, const fpoat &f) {
    return VectorV(v.x - f, v.y - f);
}

fpoat operator*(int v1, const fpoat& v2);
fpoat operator*(const fpoat& v1, int v2);
bool operator>(const fpoat& v1, int v2);
bool operator<(const fpoat& v2, int v1);

#endif /* MathHelper_h */
