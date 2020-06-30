//
//  MathHelper.cpp
//  SFML-App
//
//  Created by Jack Erb on 4/28/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#include "MathHelper.hpp"

#include <vector>
#include <math.h>
#include <iostream>

using std::cout;
using std::endl;

float clamp(float f, float l, float h) {
    if (f < l) return l;
    if (f > h) return h;
    return f;
}

fpoat dot(const VectorV &v1, const VectorV &v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

VectorV geometric_center(const PolygonV &p) {
    // Special case for circles
    if (p.size() == 2) return p[0];
    
    VectorV center;
    for (auto it = p.begin(); it != p.end(); it++) {
        center.x = center.x + it->x;
        center.y = center.y + it->y;
    }
    
    center.x = center.x / p.size();
    center.y = center.y / p.size();
    return center;
}

VectorV unit_vec(VectorV v) {
    fpoat hyp = fpsqrt((v.x * v.x) + (v.y * v.y));
    return v / hyp;
}

VectorV operator+(const VectorV &v1, const VectorV &v2) {
    return VectorV(v1.x + v2.x, v1.y + v2.y);
}

VectorV operator-(const Vector &v1, const Vector &v2) {
    return VectorV(v1.x - v2.x, v1.y - v2.y);
}

VectorV operator/(const VectorV &v, const fpoat &f) {
    return VectorV(v.x / f, v.y / f);
}

VectorV operator*(const VectorV &v, const fpoat &f) {
    return VectorV(v.x * f, v.y * f);
}

VectorV operator+(const VectorV &v, const fpoat &f) {
    return VectorV(v.x + f, v.y + f);
}

VectorV operator-(const VectorV &v, const fpoat &f) {
    return VectorV(v.x - f, v.y - f);
}

static fpoat PI_4 = FixedPoint::PI * fpoat(0,25);
static fpoat PI_3_4 = FixedPoint::PI * fpoat(0,75);

int FixedPoint::BASE = 4;
fpoat FixedPoint::PI = fpoat(3,1415);
fpoat FixedPoint::MAX = fpoat(10000000000000000);
fpoat FixedPoint::MIN = fpoat(10000000000000000, true);
fpoat FixedPoint::ZERO = fpoat(0);

fpoat fpmin(const fpoat& v1, const fpoat& v2) {
	return v1 < v2 ? v1 : v2;
}

fpoat fpmax(const fpoat& v1, const fpoat& v2) {
	return v1 > v2 ? v1 : v2;
}

fpoat fpabs(const fpoat& v) {
	return fpoat(v.n);
}

fpoat fpsin(const fpoat& v) {
	float f = v.f();
	f /= pow(10, FixedPoint::BASE);
	f = sin(f);
	f *= pow(10, FixedPoint::BASE);
	return fpoat((int)f);
}
fpoat fpcos(const fpoat& v) {
	float f = v.f();
	f /= pow(10, FixedPoint::BASE);
	f = cos(f);
	f *= pow(10, FixedPoint::BASE);
	return fpoat((int)f);
}

fpoat fpatan2(const fpoat& y, const fpoat& x) {
	float fy = y.f(), fx = x.f();

	fy /= pow(10, FixedPoint::BASE);
	fx /= pow(10, FixedPoint::BASE);
	float f = atan2(fy, fx);

	return fpoat((int)(f * pow(10, FixedPoint::BASE)));
}

fpoat fpsqrt(const fpoat& v) {
	float f = v.f();
	f /= pow(10, FixedPoint::BASE);
	f = sqrt(f);
	f *= pow(10, FixedPoint::BASE);
	return fpoat((int)f);
}

FixedPoint::FixedPoint(int fpint, int fpdec, bool s) {
	if (fpdec != 0) {
		int len = log10(fpdec) + 1;
		if (len > BASE) {
			fpdec /= pow(10, len - BASE);
		} else if (len < BASE) {
			fpdec *= pow(10, BASE - len);
		}
	}
    n = (fpint * pow(10,FixedPoint::BASE)) + fpdec;
    sign = s;
}

fpoat operator+(const fpoat &v1, const fpoat &v2) {
	long long s1 = (((int)v1.sign) * 2 - 1) * -1;
	long long s2 = (((int)v2.sign) * 2 - 1) * -1;
	long long rn = s1 * v1.n + s2 * v2.n;
    bool sign = false;
    if (rn < 0) {
        sign = true;
        rn *= -1;
    }
    return fpoat(rn, sign);
}

fpoat operator-(const fpoat &v1, const fpoat &v2) {
	long long s1 = (((int)v1.sign) * 2 - 1) * -1;
	long long s2 = (((int)v2.sign) * 2 - 1) * -1;
	long long rn = s1 * v1.n - s2 * v2.n;
    bool sign = false;
    if (rn < 0) {
        sign = true;
        rn *= -1;
    }
    return fpoat(rn, sign);
}

fpoat operator/(const fpoat &v1, const fpoat &v2) {
	long long s1 = (((int)v1.sign) * 2 - 1) * -1;
	long long s2 = (((int)v2.sign) * 2 - 1) * -1;
	long long rn = (s1 * v1.n) * pow(10, FixedPoint::BASE) / (s2 * v2.n);
    bool sign = false;
    if (rn < 0) {
        sign = true;
        rn *= -1;
    }
    return fpoat(rn, sign);
}

fpoat operator*(const fpoat &v1, const fpoat &v2) {
	long long s1 = 1;// (((int)v1.sign) * 2 - 1) * -1;
	long long s2 = 1;// (((int)v2.sign) * 2 - 1) * -1;
    long long rn = s1 * v1.n * s2 * v2.n;

	bool sign = false;
    if (rn < 0) {
        sign = true;
        rn *= -1;
    }
	rn /= pow(10, FixedPoint::BASE);

    return fpoat(rn, sign);
}

float FixedPoint::f() const {
    float s = (((int)sign) * 2 - 1) * -1;
    return s * (((float)n) / (float)pow(10,FixedPoint::BASE));
}

int FixedPoint::i() const {
	int s = (((int)sign) * 2 - 1) * -1;
	return s * (n / pow(10, FixedPoint::BASE));
}

bool operator<(const fpoat& v1, const fpoat& v2) {
	int s1 = (((int)v1.sign) * 2 - 1) * -1;
	int s2 = (((int)v2.sign) * 2 - 1) * -1;
	return s1 * v1.n < s2 * v2.n;
}
bool operator>(const fpoat& v1, const fpoat& v2) {
	int s1 = (((int)v1.sign) * 2 - 1) * -1;
	int s2 = (((int)v2.sign) * 2 - 1) * -1;
	return s1 * v1.n > s2 * v2.n;
}

bool operator>=(const fpoat& v1, const fpoat& v2) {
	return v1 > v2 || v1 == v2;
}

bool operator<=(const fpoat& v1, const fpoat& v2) {
	return v1 < v2 || v1 == v2;
}

bool operator==(const fpoat& v1, const fpoat& v2) {
	return v1.n == v2.n && v1.sign == v2.sign;
}

fpoat operator*(int v1, const fpoat& v2) {
	return fpoat(abs(v1), 0, v1 < 0) * v2;
}

fpoat operator-(const fpoat& v) {
	return fpoat(v.n, !v.sign);
}

fpoat operator*(const fpoat& v1, int v2) { return fpoat(v1.n * abs(v2), (v2 < 0) != v1.sign); }

bool operator>(const fpoat& v1, int v2) {
	return v1 > fpoat(v2, 0);
}

bool operator<(const fpoat& v2, int v1) {
	return v2 < fpoat(v1, 0);
}
