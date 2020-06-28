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

float dot(const VectorV &v1, const VectorV &v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

VectorV geometric_center(const PolygonV &p) {
    // Special case for circles
    if (p.size() == 2) return p[0];
    
    VectorV center;
    for (auto it = p.begin(); it != p.end(); it++) {
        center.x += it->x;
        center.y += it->y;
    }
    
    center.x /= p.size();
    center.y /= p.size();
    return center;
}

VectorV unit_vec(VectorV v) {
    float hyp = sqrt((v.x * v.x) + (v.y * v.y));
    return v / hyp;
}

VectorV operator+(const VectorV &v1, const VectorV &v2) {
    return VectorV(v1.x + v2.x, v1.y + v2.y);
}

VectorV operator-(const Vector &v1, const Vector &v2) {
    return VectorV(v1.x - v2.x, v1.y - v2.y);
}

VectorV operator/(const VectorV &v, float f) {
    return VectorV(v.x / f, v.y / f);
}

VectorV operator*(const VectorV &v, float f) {
    return VectorV(v.x * f, v.y * f);
}

VectorV operator+(const VectorV &v, float f) {
    return VectorV(v.x + f, v.y + f);
}

VectorV operator-(const VectorV &v, float f) {
    return VectorV(v.x - f, v.y - f);
}

int FixedPoint::BASE = 5;

FixedPoint::FixedPoint(int fpint, int fpdec, bool s) {
    n = (fpint << FixedPoint::BASE) + fpdec;
    sign = s;
}

fpoat operator+(const fpoat &v1, const fpoat &v2) {
    int s1 = (((int)v1.sign) * 2 - 1) * -1;
    int s2 = (((int)v2.sign) * 2 - 1) * -1;
    cout << "s1 " << s1 << endl;
    cout << "s2 " << s2 << endl;
    int rn = s1 * v1.n + s2 * v2.n;
    cout << rn << endl;
    bool sign = false;
    if (rn < 0) {
        sign = true;
        rn *= -1;
    }
    return fpoat(rn, sign);
}

fpoat operator-(const fpoat &v1, const fpoat &v2) {
    int s1 = (((int)v1.sign) * 2 - 1) * -1;
    int s2 = (((int)v2.sign) * 2 - 1) * -1;
    int rn = s1 * v1.n - s2 * v2.n;
    bool sign = false;
    if (rn < 0) {
        sign = true;
        rn *= -1;
    }
    return fpoat(rn, sign);
}

fpoat operator/(const fpoat &v1, const fpoat &v2) {
    int s1 = (((int)v1.sign) * 2 - 1) * -1;
    int s2 = (((int)v2.sign) * 2 - 1) * -1;
    int rn = s1 * v1.n / s2 * v2.n;
    bool sign = false;
    if (rn < 0) {
        sign = true;
        rn *= -1;
    }
    return fpoat(rn, sign);
}

fpoat operator*(const fpoat &v1, const fpoat &v2) {
    int s1 = (((int)v1.sign) * 2 - 1) * -1;
    int s2 = (((int)v2.sign) * 2 - 1) * -1;
    int rn = s1 * v1.n * s2 * v2.n;
    bool sign = false;
    if (rn < 0) {
        sign = true;
        rn *= -1;
    }
    return fpoat(rn, sign);
}

fpoat operator-(const fpoat &v1) {
    return fpoat(v1.n, !v1.sign);
}

float FixedPoint::f() {
    int s = (((int)sign) * 2 - 1) * -1;
    return s * ((float) n) / pow(10,FixedPoint::BASE);
}
