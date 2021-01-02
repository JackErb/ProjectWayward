#pragma once

#include <ww_math.h>
#include <ecs_defs.h>
#include <array>

namespace physics {
struct rigidbody {
  fp::vector position;
  std::array<fp::polygon, MaxPolygons> *polygons;
};

fp::vector check_collision(const rigidbody&, const rigidbody&);
};
