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

pair<bool, Vector2D> checkPolyCollision(const Polygon &p1, const Vector2D &pos1,
                                           const Polygon &p2, const Vector2D &pos2);

vector<Vector2D> get_orthogonals(const Polygon &p1, const Polygon &p2);

pair<bool, Vector2D>
    is_separating_axis(const Vector2D &axis,
                       const Polygon &p1, const Vector2D &pos1,
                       const Polygon &p2, const Vector2D &pos2);

bool PhysicsEngine::checkCollision(const Entity *e1, const Entity *e2, Vector2D *pv) {
    for (const Polygon& p1 : e1->polygons()) {
        for (const Polygon& p2 : e2->polygons()) {
            auto collision = checkPolyCollision(p1, e1->position(), p2, e2->position());
            if (collision.first) {
                *pv = collision.second;
                return true;
            }
        }
    }
    *pv = Vector2D();
    return false;
}

pair<bool, Vector2D> checkPolyCollision(const Polygon &p1, const Vector2D &pos1,
                                        const Polygon &p2, const Vector2D &pos2) {
    // Separating Axis Theorem
    if (p1.size() == 2 && p2.size() == 2) {
        // They are both circles
        FixedPoint x1 = p1[0].x;
        FixedPoint x2 = p2[0].x;

        Vector2D c1 = Vector2D(x1, p1[0].y) + pos1;
        Vector2D c2 = Vector2D(x2, p2[0].y) + pos2;

        FixedPoint xx = c2.x - c1.x;
        FixedPoint yy = c2.y - c1.y;
        FixedPoint dist = xx * xx + yy * yy;
        FixedPoint rad = p1[1].x + p2[1].x;
        // TODO: Calculate push vec
        if (dist < rad * rad) {
            return {true, {0,0}};
        }
        return {false, {0,0}};
    } else if (p2.size() == 2) {
        // Call with circle in first parameter
        return checkPolyCollision(p2, pos2, p1, pos1);
    }
    
    // Get the perpendicular vectors for each side of both polygons
    vector<Vector2D> orthogonals = get_orthogonals(p1, p2);
    
    // Check if there is a separating axis along each orthogonal
    vector<Vector2D> push_vectors;
    for (const auto &vec : orthogonals) {
        auto res = is_separating_axis(unit_vec(vec), p1, pos1, p2, pos2);
        if (res.first) {
            // The polygons do not collide
            return make_pair(false, Vector2D());
        } else {
            push_vectors.push_back(res.second);
        }
    }

    Vector2D min_pv = push_vectors[0];
    for (int i = 1; i < push_vectors.size(); i++) {
        Vector2D pv = push_vectors[i];
        if (dot(pv, pv) < dot(min_pv, min_pv)) min_pv = pv;
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
    
    int n = p1.size();
    if (n == 2) {
        // p1 is a circle
        
        // The closest point in p2 to p1
        FixedPoint x1 = p1[0].x;

        Vector2D vec;
        FixedPoint dist = FixedPoint::MAX;
        int i = 0;
        for (Vector2D v : p2) {
            FixedPoint xx = v.x - x1;
            FixedPoint yy = v.y - p1[0].y;
            FixedPoint d = xx * xx + yy * yy;
            if (d < dist) {
                dist = d;
                vec = v;
            }
            i++;
        }
        res.push_back(Vector2D(vec.x, vec.y) - Vector2D(x1, p1[0].y));
        res.push_back({-res[0].y,res[0].x});
    } else {
        // Get the edge vectors
        for (int i = 0; i < n; i++) {
            Vector2D pp1 = p1[(i+1) % n];
            Vector2D pp2 = p1[i];
            res.push_back(pp1 - pp2);
        }
    }
    
    // Get the edge vectors
    long n2 = p2.size();
    for (int i = 0; i < n2; i++) {
        Vector2D pp1 = p2[(i+1) % n];
        Vector2D pp2 = p2[i];
        res.push_back(pp1 - pp2);
    }
    
    // Calculate their orthogonal
    for (int i = 0; i < res.size(); i++) {
        FixedPoint x = res[i].x, y = res[i].y;
        res[i].x = -y;
        res[i].y = x;
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
        min1 = proj - p1[1].x;
        max1 = proj + p1[1].x;
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
