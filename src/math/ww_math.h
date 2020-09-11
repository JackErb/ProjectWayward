#ifndef WWMath_h
#define WWMath_h

#include <vector>
#include <iostream>

#define FP_BASE 4
#define FP_MULT 10000

struct FixedPoint {
  public:
    FixedPoint() : n(0) {}
    FixedPoint(long long n) : n(n) {}

    long long n;
    float toFloat() const;

    /* Static members */
    static FixedPoint ZERO;
    static FixedPoint MIN;
    static FixedPoint MAX;
    static FixedPoint PI;

    static FixedPoint fromInt(int n);
    static FixedPoint fromFloat(float n);
};

FixedPoint fp_atan2(const FixedPoint &y, const FixedPoint &x);
FixedPoint fp_sqrt(const FixedPoint &n);
FixedPoint fp_min(const FixedPoint &n1, const FixedPoint &n2);
FixedPoint fp_max(const FixedPoint &n1, const FixedPoint &n2);
FixedPoint fp_abs(const FixedPoint &n);
FixedPoint fp_sign(const FixedPoint &n);

struct Vector2D {
    Vector2D() : x(), y() {}
    Vector2D(FixedPoint x, FixedPoint y) : x(x), y(y) {}

    FixedPoint x;
    FixedPoint y;
};

struct Circle {
    Vector2D position;
    FixedPoint radius;
};

typedef std::vector<Vector2D> Polygon;
Polygon poly_square(int nx, int ny, int nw, int nh);
Polygon poly_circle(int nx, int ny, int nr);

Vector2D unit_vec(const Vector2D &v);
FixedPoint dot(const Vector2D &v1, const Vector2D &v2);
Vector2D geometric_center(const Polygon &p);
FixedPoint fp_dist(const Vector2D &v1, const Vector2D &v2);
FixedPoint fp_distsqr(const Vector2D &v1, const Vector2D &v2);
FixedPoint fp_magnitude(const Vector2D &v);

#endif  /* WWMath_h */
