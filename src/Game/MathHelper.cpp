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

typedef std::vector<VectorV> PolygonV;

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
