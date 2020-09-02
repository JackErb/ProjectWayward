#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>

using std::pair;
using std::make_pair;
using std::cout;
using std::endl;
using std::min;
using std::max;

vector<Vector2D> get_orthogonals(const Polygon &p1, const Polygon &p2);

pair<bool, Vector2D>
    is_separating_axis(const Vector2D &axis,
                       const Polygon &p1, const Vector2D &pos1,
                       const Polygon &p2, const Vector2D &pos2);

bool PhysicsEngine::CheckCollision(const Entity *e1, const Entity *e2) {
    for (const Polygon& p1 : e1->polygons) {
        for (const Polygon& p2 : e2->polygons) {
            if (checkCollision(p1, e1->Position(), p2, e2->Position()).first) {
                return true;
            }
        }
    }
    return false;
}

pair<bool, Vector2D>
  PhysicsEngine::checkCollision(const Polygon &p1, const Vector2D &pos1,
                                const Polygon &p2, const Vector2D &pos2) {
    // Separating Axis Theorem
    if (p1.size() == 2 && p2.size() == 2) {
        // They are both circles
		fpoat x1 = p1[0].x;
		fpoat x2 = p2[0].x;

        Vector2D c1 = Vector2D(x1, p1[0].y) + pos1;
        Vector2D c2 = Vector2D(x2, p2[0].y) + pos2;

		fpoat xx = c2.x - c1.x;
		fpoat yy = c2.y - c1.y;
		fpoat dist = xx * xx + yy * yy;
		fpoat rad = p1[1].x + p2[1].x;
        // TODO: Calculate push vec
        if (dist < rad * rad) {
            return {true, {0,0}};
        }
        return {false, {0,0}};
    } else if (p2.size() == 2) {
        // Call with circle in first parameter
        return checkCollision(p2, pos2, dir2, p1, pos1, dir1);
    }
    
    // Get the perpendicular vectors for each side of both polygons
    vector<Vector2D> orthogonals = get_orthogonals(p1, dir1, p2, dir2);
    
    // Check if there is a separating axis along each orthogonal
    vector<Vector2D> push_vectors;
    for (const auto &vec : orthogonals) {
        auto res = is_separating_axis(unit_vec(vec), p1, pos1, dir1, p2, pos2, dir2);
        if (res.first) {
            // The polygons do not collide
            return make_pair(false, Vector2D(0,0));
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
    Vector2D displacement = (geometric_center(p2, dir2) + pos2) -
      (geometric_center(p1, dir1) + pos1);
    if (dot(displacement, min_pv) > 0) {
		min_pv.x.sign = !min_pv.x.sign;
        min_pv.y.sign = !min_pv.y.sign;
    }
    
    return make_pair(true, min_pv);
}

/* Returns the orthogonal vectors of all sides of this polygon. */
vector<Vector2D> get_orthogonals(const Polygon &p1, const PolygonV &p2) {
    vector<Vector2D> res;
    
    int n = p1.size();
    if (n == 2) {
        // p1 is a circle
        
        // The closest point in p2 to p1
		fpoat x1 = p1[0].x;

        Vector2D vec;
		fpoat dist = fpoat::MAX;
        int i = 0;
        for (Vector2D v : p2) {
			fpoat xx = v.x - x1;
			fpoat yy = v.y - p1[0].y;
			fpoat d = xx * xx + yy * yy;
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
        fpoat x = res[i].x, y = res[i].y;
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
    fpoat min1 = FixedPoint::MAX,
          max1 = FixedPoint::MIN,
          min2 = min1,
          max2 = max1;
        
    if (p1.size() == 2) {
        // p1 is a circle
		fpoat x = p1[0].x;
		if (dir1 == -1) x.sign = !x.sign;

        fpoat proj = dot(axis, Vector2D(x, p1[0].y) + pos1);
        min1 = proj - p1[1].x;
        max1 = proj + p1[1].x;
    } else {
        for (const Vector2D &vert : p1) {
			fpoat x = vert.x;
			if (dir1 == -1) x.sign = !x.sign;

            fpoat proj = dot(axis, Vector2D(x, vert.y) + pos1);
            min1 = fpmin(proj, min1);
            max1 = fpmax(proj, max1);
        }
    }

    for (const Vector2D &vert : p2) {
		fpoat x = vert.x;
		if (dir2 == -1) x.sign = !x.sign;

        fpoat proj = dot(axis, Vector2D(x, vert.y) + pos2);
        min2 = fpmin(proj, min2);
        max2 = fpmax(proj, max2);
    }
    
    if (max1 >= min2 && max2 >= min1) {
        // Calculate push vector
        fpoat d = fpmin(max2 - min1, max1 - min2);
        Vector2D push_vector(axis * (d / dot(axis, axis) + fpoat(1, d.sign)));
        return make_pair(false, push_vector);
    } else {
        return make_pair(true, Vector2D(fpoat(0),fpoat(0)));
    }
}
