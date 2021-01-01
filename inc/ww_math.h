#pragma once

#include <vector>
#include <limits>

#define FP_BASE 4
#define FP_MULT 10000

namespace fp {

typedef struct fixed_point {
  fixed_point() : n(0) {}
  fixed_point(long long n) : n(n) {}

  long long n;
} fixed_point;

const fixed_point ZERO = {0};
const fixed_point MIN  = {std::numeric_limits<long long>().min()};
const fixed_point MAX  = {std::numeric_limits<long long>().max()};
const fixed_point PI   = {31416};

fixed_point from_int(int);
fixed_point from_float(float);

float to_float(fixed_point);
int   to_int(fixed_point);

fixed_point atan2(fixed_point, fixed_point);
fixed_point sqrt(fixed_point);
fixed_point min(fixed_point, fixed_point);
fixed_point max(fixed_point, fixed_point);
fixed_point abs(fixed_point);
fixed_point sign(fixed_point);

typedef struct vector {
  vector() : x(), y() {}
  vector(fixed_point x, fixed_point y) : x(x), y(y) {}

  fixed_point x;
  fixed_point y;
} vector;

typedef struct circle {
  vector position;
  fixed_point radius;
} circle;

typedef std::vector<vector> polygon;
// x y w h
polygon poly_square(int, int, int ,int);
// x y r
polygon poly_circle(int, int, int);

vector unit_vec(const vector&);
fixed_point dot(const vector&, const vector&);
vector geometric_center(const polygon&);
fixed_point dist(const vector&, const vector&);
fixed_point distsqr(const vector&, const vector&);
fixed_point magnitude(const vector &v);

};
