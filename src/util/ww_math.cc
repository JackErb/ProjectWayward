#include "ww_math.h"

#include "fp_overloads.h"
#include "vector_overloads.h"
#include <iostream>
#include <cstdlib>
#include <math.h>
#include <logs.h>

using fp::fixed_point;
using fp::vector;
using fp::polygon;
using std::cout;
using std::cerr;
using std::endl;

vector unit_vec(const vector &v) {
  return v / fp::sqrt(v.x * v.x + v.y * v.y);
}

vector geometric_center(const polygon &p) {
  // Special case for circles
  fatalerror("geometric_center() : not implemented");
  return vector();
}

fixed_point fp::dist(const vector &v1, const vector &v2) {
  return fp::sqrt(fp::distsqr(v1, v2));
}

fixed_point fp::distsqr(const vector &v1, const vector &v2) {
  fixed_point x = v1.x - v2.x;
  fixed_point y = v1.y - v2.y;
  return x * x + y * y;
}

polygon fp::make_rectangle(int x, int y, int w, int h) {
  polygon rect;
  rect.type = poly_rectangle;

  fixed_point fx = fp::from_int(x),  fy = fp::from_int(y);
  fixed_point fw = fp::from_int(w), fh = fp::from_int(h);
  rect.rectangle.position = {fx, fy};
  rect.rectangle.size = {fw, fh};
  return rect;
}

polygon fp::make_circle(int x, int y, int r) {
  polygon circle;
  circle.type = poly_circle;

  circle.circle.position = {fp::from_int(x), fp::from_int(y)};
  circle.circle.radius = fp::from_int(r);
  return circle;
}

fixed_point fp::dot(const vector &v1, const vector &v2) {
  return v1.x * v2.x + v1.y * v2.y;
}


float fp::to_float(fixed_point n) {
  return (float)n.n / FP_MULT;
}

int fp::to_int(fixed_point n) {
  return n.n / FP_MULT;
}

fixed_point fp::from_float(float n) {
  return fixed_point((int)(n * FP_MULT));
}

fixed_point fp::from_int(int n) {
  return fixed_point(n * FP_MULT);
}


float temp_atan2(float y, float x) {
  return atan2(-y, x);
}

fixed_point fp::atan2(fixed_point y, fixed_point x) {
  float fy = fp::to_float(y), fx = fp::to_float(x);
  float f = temp_atan2(fy, fx);
  return fp::from_float(f);
}

float temp_sqrt(float n) {
  return sqrt(n);
}

fixed_point fp::sqrt(fixed_point n) {
  float fn = fp::to_float(n);
  float f = temp_sqrt(fn);
  return fp::from_float(f);
}

fixed_point fp::min(fixed_point n1, fixed_point n2) {
  return n1 < n2 ? n1 : n2;
}

fixed_point fp::max(fixed_point n1, fixed_point n2) {
  return n1 > n2 ? n1 : n2;
}

long long temp_abs(long long n) { return abs(n); }

fixed_point fp::abs(fixed_point n) {
  return fixed_point(temp_abs(n.n));
}

fixed_point fp::sign(fixed_point n) {
  return n.n < 0 ? fp::from_int(-1) : fp::from_int(1);
}

fixed_point fp::magnitude(const vector &v) {
  return fp::sqrt(v.x * v.x + v.y * v.y);
}
