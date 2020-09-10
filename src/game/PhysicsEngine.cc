#include "PhysicsEngine.h"
#include "Entity.h"

#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <ww_math.h>

using std::pair;
using std::make_pair;
using std::cout;
using std::endl;
using std::min;
using std::max;
using std::vector;

bool checkBoundsCollision(const Circle &c1, const Vector2D &pos1,
                          const Circle &c2, const Vector2D &pos2);

pair<bool, Vector2D> checkPolyCollision(const Polygon &p1, const Vector2D &pos1,
                                        const Polygon &p2, const Vector2D &pos2);

vector<Vector2D> get_orthogonals(const Polygon &p1, const Polygon &p2);

pair<bool, Vector2D>
    is_separating_axis(const Vector2D &axis,
                       const Polygon &p1, const Vector2D &pos1,
                       const Polygon &p2, const Vector2D &pos2);

bool PhysicsEngine::checkCollision(Entity *e1, Entity *e2, Vector2D *pv) {
    const CollisionBox &hurtbox_e1 = e1->polygons_hurt();
    const CollisionBox &hurtbox_e2 = e2->polygons_hurt();

    Vector2D pos_e1 = e1->position();
    Vector2D pos_e2 = e2->position();

    for (int i = 0; i < hurtbox_e1.polys.size(); i++) {
        for (int j = 0; j < hurtbox_e2.polys.size(); j++) {
            // Bounds check before more expensive polygon check
            bool bounds_check = checkBoundsCollision(hurtbox_e1.bounds[i], pos_e1,
                                                     hurtbox_e2.bounds[j], pos_e2);
            if (!bounds_check) {
                continue;
            }

            auto collision = checkPolyCollision(hurtbox_e1.polys[i], pos_e1,
                                                hurtbox_e2.polys[j], pos_e2);
            if (collision.first) {
                *pv = collision.second;
                return true;
            }
        }
    }
    return false;
}

bool PhysicsEngine::checkHitboxCollision(Entity *e1, Entity *e2, Vector2D *pv) {
    const CollisionBox &hitbox_e1 = e1->polygons_hit();
    const CollisionBox &hurtbox_e2 = e2->polygons_hurt();

    for (const Polygon &p1 : hitbox_e1.polys) {
        for (const Polygon &p2 : hurtbox_e2.polys) {
            auto collision = checkPolyCollision(p1, e1->position(), p2, e2->position());
            if (collision.first) {
                *pv = collision.second;
                return true;
            }
        }
    }
    return false;
}

bool checkBoundsCollision(const Circle &c1, const Vector2D &pos1,
                          const Circle &c2, const Vector2D &pos2) {
    Vector2D center1 = c1.position + pos1;
    Vector2D center2 = c2.position + pos2;

    FixedPoint total_radius = c1.radius * c1.radius + c2.radius * c2.radius;
    return fp_distsqr(center1, center2) < total_radius;
}

pair<bool, Vector2D> checkPolyCollision(const Polygon &p1, const Vector2D &pos1,
                                        const Polygon &p2, const Vector2D &pos2) {
    // Separating Axis Theorem
    if (p1.size() == 2 && p2.size() == 2) {
        // They are both circles
        Vector2D c1 = p1[0] + pos1, c2 = p2[0] + pos2;
        FixedPoint x = c2.x - c1.x, y = c2.y - c1.y;
        FixedPoint dist = x * x + y * y;
        FixedPoint rad = p1[1].x + p2[1].x;
        if (dist < rad * rad) {
            // TODO: Calculate push vec
            Vector2D pv;
            return {true, pv};
        }
        return {false, Vector2D()};
    } else if (p2.size() == 2) {
        // Call with circle in first parameter
        return checkPolyCollision(p2, pos2, p1, pos1);
    }
    
    // Get the perpendicular vectors for each side of both polygons
    vector<Vector2D> orthogonals = get_orthogonals(p1, p2);
    
    // Check if there is a separating axis along each orthogonal
    vector<Vector2D> push_vectors;
    Vector2D min_pv;
    FixedPoint min_dot = FixedPoint::MAX;
    for (const Vector2D &vec : orthogonals) {
        auto res = is_separating_axis(unit_vec(vec), p1, pos1, p2, pos2);
        if (res.first) {
            // The polygons do not collide
            return make_pair(false, Vector2D());
        } else {
            Vector2D pv = res.second;
            FixedPoint pv_dot = dot(pv, pv);
            if (pv_dot < min_dot) {
                min_dot = pv_dot;
                min_pv = pv;
            }
        }
    }

    // Check that the push vector is pointing the right direction
    Vector2D displacement = (geometric_center(p2) + pos2) - (geometric_center(p1) + pos1);
    if (dot(displacement, min_pv) > 0) {
        min_pv.x = -min_pv.x;
        min_pv.y = -min_pv.y;
    }
    
    return make_pair(true, min_pv);
}

/* Returns the orthogonal vectors of all sides of this polygon. */
vector<Vector2D> get_orthogonals(const Polygon &p1, const Polygon &p2) {
    vector<Vector2D> res;
    
    int p1_size = p1.size();
    if (p1_size == 2) {
        // p1 is a circle

        // Find the closest point in p2 to p1
        Vector2D point;
        FixedPoint min_dist = FixedPoint::MAX;
        for (Vector2D v : p2) {
            FixedPoint x = v.x - p1[0].x;
            FixedPoint y = v.y - p1[0].y;
            FixedPoint dist = x * x + y * y;
            if (dist < min_dist) {
                min_dist = dist;
                point = v;
            }
        }
        // The axis between center of the circle and the closest
        // point in p2
        Vector2D axis = point - p1[0];
        res.push_back(axis);
        res.push_back(orthogonal(axis));
    } else {
        // Get the edge vector
        for (int i = 0; i < p1_size; i++) {
            Vector2D axis = p1[(i+1) % p1_size] - p1[i];
            res.push_back(orthogonal(axis));
        }
    }
    
    // Get the edge vectors
    int p2_size = p2.size();
    for (int i = 0; i < p2_size; i++) {
        Vector2D axis = p2[(i+1) % p2_size] - p2[i];
        res.push_back(orthogonal(axis));
    }

    return res;
}

/* Calculate if the given axis is a separating axis for the two polygons.
 * If it is, returns a pair (true, pv) where pv is the push vector
 * If not, returns the pair (false, pv) where pv is (0,0)
 */
pair<bool, Vector2D> is_separating_axis(const Vector2D &axis,
                                        const Polygon &p1, const Vector2D &pos1,
                                        const Polygon &p2, const Vector2D &pos2) {
    FixedPoint min1 = FixedPoint::MAX,
               max1 = FixedPoint::MIN,
               min2 = min1,
               max2 = max1;
        
    if (p1.size() == 2) {
        // p1 is a circle
        FixedPoint proj = dot(axis, p1[0] + pos1);
        FixedPoint radius = p1[1].x;
        min1 = proj - radius;
        max1 = proj + radius;
    } else {
        for (const Vector2D &vert : p1) {
            FixedPoint proj = dot(axis, vert + pos1);
            min1 = fp_min(proj, min1);
            max1 = fp_max(proj, max1);
        }
    }

    for (const Vector2D &vert : p2) {
        FixedPoint proj = dot(axis, vert + pos2);
        min2 = fp_min(proj, min2);
        max2 = fp_max(proj, max2);
    }
    
    if (max1 >= min2 && max2 >= min1) {
        // Calculate push vector
        FixedPoint d = fp_min(max2 - min1, max1 - min2);
        FixedPoint epsilon = FixedPoint(1);
        if (d < FixedPoint(0)) epsilon = -epsilon;

        Vector2D push_vector(axis * (d / dot(axis, axis) + epsilon));
        return make_pair(false, push_vector);
    } else {
        return make_pair(true, Vector2D(FixedPoint(0),FixedPoint(0)));
    }
}
