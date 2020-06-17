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

VectorV operator+(const VectorV &v1, const VectorV &v2);
VectorV operator-(const VectorV &v1, const VectorV &v2);
VectorV operator/(const VectorV &v, float f);
VectorV operator*(const VectorV &v, float f);
VectorV operator+(const VectorV &v, float f);
VectorV operator-(const VectorV &v, float f);


float clamp(float f, float l, float h);
float dot(const VectorV &v1, const VectorV &v2);
VectorV geometric_center(const PolygonV &p);
VectorV unit_vec(VectorV v);


#endif /* MathHelper_h */
