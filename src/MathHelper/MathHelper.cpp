//
//  MathHelper.cpp
//  SFML-App
//
//  Created by Jack Erb on 4/28/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#include "../MathHelper/MathHelper.h"

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

VectorV geometric_center(const PolygonV &p, int dir) {
    // Special case for circles
	if (p.size() == 2) {
		VectorV v = p[0];
		if (dir == -1) v.x.sign = !v.x.sign;
		return v;
	}
    
    VectorV center;
    for (auto it = p.begin(); it != p.end(); it++) {
		fpoat x = it->x;
		if (dir == -1) x.sign = !x.sign;

        center.x = center.x + x;
        center.y = center.y + it->y;
    }
    
    center.x = center.x / fpoat(p.size(),0);
    center.y = center.y / fpoat(p.size(),0);
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

int FixedPoint::BASE = 2;
int FixedPoint::MULT = 100;
fpoat FixedPoint::PI = FixedPoint::FromFloat(3.14159);
fpoat FixedPoint::MAX = fpoat(10000000000000000);
fpoat FixedPoint::MIN = fpoat(10000000000000000, true);
fpoat FixedPoint::ZERO = fpoat(0);

fpoat FixedPoint::FromFloat(float f) {
	int fp = abs(f) * MULT;
	return fpoat(fp, f < 0);
}

fpoat FixedPoint::FromInt(int n) {
    return fpoat(abs(n), 0, n < 0);
}

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
	f = sin(f);
	return FixedPoint::FromFloat(f);
}
fpoat fpcos(const fpoat& v) {
	float f = v.f();
	f = cos(f);
	return FixedPoint::FromFloat(f);
}

fpoat fpatan2(const fpoat& y, const fpoat& x) {
	float fy = y.f(), fx = x.f();

	float f = atan2(-fy, fx);

	return FixedPoint::FromFloat(f);
}

fpoat fpsqrt(const fpoat& v) {
	float f = v.f();
	f = sqrt(f);
	return FixedPoint::FromFloat(f);
}

FixedPoint::FixedPoint(int fpint, int fpdec, bool s) {
	if (fpdec != 0) {
		int len = log10(fpdec) + 1;
		if (len > BASE) {
			for (int i = 0; i < abs(len - BASE); i++) {
				fpdec /= 10;
			}
		} else if (len < BASE) {
			for (int i = 0; i < abs(len - BASE); i++) {
				fpdec *= 10;
			}
		}
	}
    n = (fpint * MULT) + fpdec;
    sign = s;
}

fpoat operator+(const fpoat &v1, const fpoat &v2) {
    long long rn = (v1.sign ? -v1.n : v1.n) + (v2.sign ? -v2.n : v2.n);
    bool sign = rn < 0;
    return fpoat(abs(rn), sign);
}

fpoat operator-(const fpoat &v1, const fpoat &v2) {
    long long rn = (v1.sign ? -v1.n : v1.n) - (v2.sign ? -v2.n : v2.n);
    bool sign = rn < 0;
    return fpoat(abs(rn), sign);
}

fpoat operator/(const fpoat &v1, const fpoat &v2) {
    if (v2.n == 0) {
        std::cerr << "ERROR DIVIDE BY 0" << std::endl;
        return fpoat(0);
    }
	long long rn = v1.n * FixedPoint::MULT / v2.n;
    return fpoat(rn, v1.sign != v2.sign);
}

fpoat operator*(const fpoat &v1, const fpoat &v2) {
    long long rn = v1.n * v2.n / FixedPoint::MULT;
    return fpoat(rn, v1.sign != v2.sign);
}

float FixedPoint::f() const {
    float s = (((int)sign) * 2 - 1) * -1;
    return s * ((double)n / (double)MULT);
}

int FixedPoint::i() const {
	int s = (((int)sign) * 2 - 1) * -1;
	return s * (n / pow(10, FixedPoint::BASE));
}

bool operator<(const fpoat& v1, const fpoat& v2) {
	long long s1 = (((long long)v1.sign) * 2 - 1) * -1;
	long long s2 = (((long long)v2.sign) * 2 - 1) * -1;
	return s1 * v1.n < s2 * v2.n;
}
bool operator>(const fpoat& v1, const fpoat& v2) {
	long long s1 = (((long long)v1.sign) * 2 - 1) * -1;
	long long s2 = (((long long)v2.sign) * 2 - 1) * -1;
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
