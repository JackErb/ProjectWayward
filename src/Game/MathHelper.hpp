//
//  MathHelper.h
//  SFML-App
//
//  Created by Jack Erb on 4/28/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef MathHelper_hpp
#define MathHelper_hpp

#define PI 3.1415926535f

#include <vector>

typedef struct Vector {
    Vector() : x(0), y(0) {}
    Vector(float x, float y) : x(x), y(y) {}
    
    float x, y;
} VectorV;

typedef std::vector<VectorV> PolygonV;

typedef struct FixedPoint {
public:
    static int BASE;
    FixedPoint(int fpint, int fpdec, bool s = false);
    FixedPoint(int n, bool s = false) : n(n), sign(s) {}
    
    long n;
    bool sign;
    
    // Get the float representation
    // DO NOT USE THIS FOR GAMEPLAY PURPOSES! Only things that can
    // be non deterministic, i.e. graphics
    float f();
} fpoat;

float clamp(float f, float l, float h);

VectorV unit_vec(VectorV v);

float dot(const VectorV &v1, const VectorV &v2);

VectorV geometric_center(const PolygonV &p);

VectorV operator+(const VectorV &v1, const VectorV &v2);
VectorV operator-(const VectorV &v1, const VectorV &v2);
VectorV operator/(const VectorV &v, float f);
VectorV operator*(const VectorV &v, float f);
VectorV operator+(const VectorV &v, float f);
VectorV operator-(const VectorV &v, float f);

fpoat operator+(const fpoat &v1, const fpoat &v2);
fpoat operator-(const fpoat &v1, const fpoat &v2);
fpoat operator/(const fpoat &v1, const fpoat &v2);
fpoat operator*(const fpoat &v1, const fpoat &v2);
fpoat operator-(const fpoat &v1);

#endif /* MathHelper_h */
