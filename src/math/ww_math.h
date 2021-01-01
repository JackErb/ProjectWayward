#pragma once

#include <vector>
#include <iostream>

#define FP_BASE 4
#define FP_MULT 10000

namespace fp {

typedef struct fixed_point {
  long long n;
} fixed_point;

const fixed_point ZERO;
const fixed_point MIN;
const fixed_point MAX;
const fixed_point PI;

fixed_point from_int(int);
fixed_point from_float(float);

float to_float(fixed_point);
float to_int(fixed_point);

fixed_point atan2(fixed_point, fixed_point);
fixed_point sqrt(fixed_point);
fixed_point min(fixed_point, fixed_point);
fixed_point max(fixed_point, fixed_point);
fixed_point abs(fixed_point);
fixed_point sign(fixed_point);

typedef struct vector {
  fixed_point x;
  fixed_point y;
} vector;

typedef struct circle {
  vector position;
  fixed_point radius;
} circle;

typedef std::vector<vector> polygon;
// x y w h
Polygon poly_square(int, int, int ,int);
// x y r
Polygon poly_circle(int, int, int);

Vector2D unit_vec(const Vector2D &v);
fixed_point dot(vector, vector);
Vector2D geometric_center(polygon;
fixed_point dist(vector, vector);
fixed_point distsqr(vector, vector);
fixed_point magnitude(const Vector2D &v);

}
