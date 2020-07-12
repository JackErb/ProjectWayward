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

VectorV operator+(const VectorV &v1, const VectorV &v2);
VectorV operator-(const VectorV &v1, const VectorV &v2);
VectorV operator/(const VectorV &v, const fpoat &f);
VectorV operator*(const VectorV &v, const fpoat &f);
VectorV operator+(const VectorV &v, const fpoat &f);
VectorV operator-(const VectorV &v, const fpoat &f);

fpoat operator+(const fpoat &v1, const fpoat &v2);
fpoat operator-(const fpoat &v1, const fpoat &v2);
fpoat operator/(const fpoat &v1, const fpoat &v2);
fpoat operator*(const fpoat &v1, const fpoat &v2);
fpoat operator-(const fpoat &v1);

fpoat operator*(int v1, const fpoat& v2);
fpoat operator*(const fpoat& v1, int v2);

bool operator>(const fpoat& v1, const fpoat& v2);
bool operator<(const fpoat& v1, const fpoat& v2);
bool operator>=(const fpoat& v1, const fpoat& v2);
bool operator<=(const fpoat& v1, const fpoat& v2);
bool operator==(const fpoat& v1, const fpoat& v2);

bool operator>(const fpoat& v1, int v2);
bool operator<(const fpoat& v2, int v1);

fpoat operator-(const fpoat& v);

#endif /* MathHelper_h */
