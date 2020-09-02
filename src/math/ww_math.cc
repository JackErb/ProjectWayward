#include "ww_math.h"

#include <iostream>
#include <limits.h>

using std::cout;
using std::cerr;
using std::endl;

Vector2D geometricCenter(const Polygon &p) {
    // Special case for circles
    if (p.size() == 2) {
        Vector2D v = p[0];
        return v;
    }

    Vector2D center = {0, 0};
    for (auto it = p.begin(); it != p.end(); it++) {
        center.x = center.x + it->x;
        center.y = center.y + it->y;
    }

	FixedPoint size = FixedPoint::fromInt(p.size());
    center.x = center.x / size;
    center.y = center.y / size;
    return center;
}

FixedPoint FixedPoint::MIN = FixedPoint(LLONG_MAX);
FixedPoint FixedPoint::MAX = FixedPoint(LLONG_MIN);
FixedPoint FixedPoint::PI = FixedPoint::fromFloat(3.14159);

int FixedPoint::BASE = 4;
int FixedPoint::MULT = 10000;

float FixedPoint::toFloat() {
	return (float)this->n / MULT;	
}

FixedPoint FixedPoint::fromFloat(float n) {
	return FixedPoint((int)(n * MULT));
}

FixedPoint FixedPoint::fromInt(int n) {
	return FixedPoint(n * MULT);
}
