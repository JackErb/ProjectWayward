#include "ww_math.h"

#include "fp_overloads.h"
#include "vector_overloads.h"
#include <iostream>
#include <cstdlib>
#include <math.h>

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
  if (p.size() == 2) {
    vector v = p[0];
    return v;
  }

  vector center = {fp::ZERO, fp::ZERO};
  for (auto it = p.begin(); it != p.end(); it++) {
    center.x += it->x;
    center.y += it->y;
  }

  fixed_point size = fp::from_int(p.size());
  center.x = center.x / size;
  center.y = center.y / size;
  return center;
}

fixed_point fp::dist(const vector &v1, const vector &v2) {
  return fp::sqrt(fp::distsqr(v1, v2));
}

fixed_point fp::distsqr(const vector &v1, const vector &v2) {
  fixed_point x = v1.x - v2.x;
  fixed_point y = v1.y - v2.y;
  return x * x + y * y;
}

polygon fp::poly_square(int nx, int ny, int nw, int nh) {
  polygon poly;

  fixed_point x = fp::from_int(nx),
         y = fp::from_int(ny),
         w = fp::from_int(nw),
         h = fp::from_int(nh);

  fixed_point TWO = fp::from_int(2);
  poly.push_back(vector(x - w / TWO, y - h / TWO));
  poly.push_back(vector(x + w / TWO, y - h / TWO));
  poly.push_back(vector(x + w / TWO, y + h / TWO));
  poly.push_back(vector(x - w / TWO, y + h / TWO));
  return poly;
}

polygon fp::poly_circle(int nx, int ny, int nr) {
  polygon poly;

  fixed_point x = fp::from_int(nx),
        y = fp::from_int(ny),
        r = fp::from_int(nr);
  poly.push_back(vector(x, y));
  poly.push_back(vector(r, 0));
  return poly;
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

fixed_point fp::abs(fixed_point n) {
  return fixed_point(abs(n.n));
}

fixed_point fp::sign(fixed_point n) {
  return n.n < 0 ? fp::from_int(-1) : fp::from_int(1);
}

fixed_point fp::magnitude(const vector &v) {
  return fp::sqrt(v.x * v.x + v.y * v.y);
}
