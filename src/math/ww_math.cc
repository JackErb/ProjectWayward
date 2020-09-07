#include "ww_math.h"

#include "fp_overloads.h"
#include "vector_overloads.h"
#include <iostream>
#include <limits.h>
#include <cstdlib>
#include <math.h>

using std::cout;
using std::cerr;
using std::endl;

Vector2D unit_vec(const Vector2D &v) {
    FixedPoint hyp = fp_sqrt(v.x * v.x + v.y * v.y);
    return v / hyp; 
}

Vector2D geometric_center(const Polygon &p) {
    // Special case for circles
    if (p.size() == 2) {
        Vector2D v = p[0];
        return v;
    }

    Vector2D center = {FixedPoint::ZERO, FixedPoint::ZERO};
    for (auto it = p.begin(); it != p.end(); it++) {
        center.x += it->x;
        center.y += it->y;
    }

    FixedPoint size = FixedPoint::fromInt(p.size());
    center.x = center.x / size;
    center.y = center.y / size;
    return center;
}

Polygon poly_square(int nx, int ny, int nw, int nh) {
    Polygon poly;

    FixedPoint x = FixedPoint::fromInt(nx),
               y = FixedPoint::fromInt(ny),
               w = FixedPoint::fromInt(nw),
               h = FixedPoint::fromInt(nh);

    FixedPoint TWO = FixedPoint::fromInt(2);
    poly.push_back(Vector2D(x - w / TWO, y - h / TWO));
    poly.push_back(Vector2D(x + w / TWO, y - h / TWO));
    poly.push_back(Vector2D(x + w / TWO, y + h / TWO));
    poly.push_back(Vector2D(x - w / TWO, y + h / TWO));
    return poly;
}

FixedPoint dot(const Vector2D &v1, const Vector2D &v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

FixedPoint FixedPoint::ZERO = FixedPoint(0);
FixedPoint FixedPoint::MIN = FixedPoint(LLONG_MIN);
FixedPoint FixedPoint::MAX = FixedPoint(LLONG_MAX);
FixedPoint FixedPoint::PI = FixedPoint::fromFloat(3.14159f);

int FixedPoint::BASE = 4;
int FixedPoint::MULT = 10000;

float FixedPoint::toFloat() const {
    return (float)this->n / MULT;   
}

FixedPoint FixedPoint::fromFloat(float n) {
    return FixedPoint((int)(n * MULT));
}

FixedPoint FixedPoint::fromInt(int n) {
    return FixedPoint(n * MULT);
}



FixedPoint fp_atan2(const FixedPoint &y, const FixedPoint &x) {
    float fy = y.toFloat(), fx = x.toFloat();
    float f = atan2(-fy, fx);
    return FixedPoint::fromFloat(f);
}

FixedPoint fp_sqrt(const FixedPoint &n) {
    float fn = n.toFloat();
    float f = sqrt(fn);
    return FixedPoint::fromFloat(f);
}

FixedPoint fp_min(const FixedPoint &n1, const FixedPoint &n2) {
    return n1 < n2 ? n1 : n2;
}

FixedPoint fp_max(const FixedPoint &n1, const FixedPoint &n2) {
    return n1 > n2 ? n1 : n2;
}

FixedPoint fp_abs(const FixedPoint &n) {
    return FixedPoint(abs(n.n));
}

FixedPoint fp_sign(const FixedPoint &n) {
    return n.n < 0 ? FixedPoint::fromInt(-1) : FixedPoint::fromInt(1);
}
