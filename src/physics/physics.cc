#include <physics.h>
#include <logs.h>
#include <ww_math.h>
#include <fp_overloads.h>
#include <vector_overloads.h>
#include <iostream>

using namespace physics;
using fp::fixed_point;
using fp::vector;
using fp::polygon;

vector push_vector(const vector &p1, const polygon &poly1,
                   const vector &p2, const polygon &poly2) {
  if (poly1.type == fp::poly_circle || poly2.type == fp::poly_circle) {
    fatalerror("check_collision() : circle collisions not implemented");
  }

  vector pos1 = p1 + poly1.rectangle.position;
  vector size1 = poly1.rectangle.size;
  vector pos2 = p2 + poly2.rectangle.position;
  vector size2 = poly2.rectangle.size;

  fixed_point diffx = pos1.x - pos2.x;
  fixed_point px = (size1.x + size2.x) / fp::from_int(2) - fp::abs(diffx);
  if (px <= 0) {
    return vector();
  }

  fixed_point diffy = pos1.y - pos2.y;
  fixed_point py = (size1.y + size2.y) / fp::from_int(2) - fp::abs(diffy);
  if (py <= 0) {
    return vector();
  }

  if (px < py) {
    return vector(fp::sign(diffx) * px, 0);
  } else {
    return vector(0, fp::sign(diffy) * py);
  }
}

vector physics::check_collision(const rigidbody &r1, const rigidbody &r2) {
  for (const polygon &p1 : *r1.polygons) {
    for (const polygon &p2 : *r2.polygons) {
      vector pv = push_vector(r1.position, p1, r2.position, p2);
      if (pv.x != fp::ZERO || pv.y != fp::ZERO) {
        // Push vector is non-zero, meaning they are colliding.
        return pv;
      }
    }
  }
  // No collision detected
  return vector();
}
