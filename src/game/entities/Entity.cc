#include "Entity.h"
#include <WaywardGL.h>
#include "SpriteBuffer.h"
#include "GameController.h"

using std::vector;
using std::iterator;

void flipPolys(CollisionBox &box) {
    for (Polygon &poly : box.polys) {
        if (poly.size() == 2) {
            // Special case for circles
            poly[0].x = -poly[0].x;
            continue;
        }
        for (Vector2D &vec : poly) {
            vec.x = -vec.x;
        }
    }

    for (Circle &bound : box.bounds) {
        bound.position.x = -bound.position.x;
    }
}

static CollisionBox Empty;

const CollisionBox& Entity::polygons_hurt() {
    if (data.hurtbox_handle == -1) return Empty;
    CollisionBox &box = hurtboxes[data.hurtbox_handle];
    if (data.dir != box.dir) {
        flipPolys(box);
        box.dir = data.dir;
    }
    return box;
}

const CollisionBox& Entity::polygons_hit() {
    if (data.hitbox_handle == -1) return Empty;
    CollisionBox &box = hitboxes[data.hitbox_handle];
    if (data.dir != box.dir) {
        flipPolys(box);
        box.dir = data.dir;
    }
    return box;
}

Circle getBounds(const Polygon &poly) {
    Vector2D center = geometric_center(poly);
    FixedPoint max_dist = FixedPoint::ZERO;
    for (const Vector2D &vec : poly) {
        FixedPoint dist = fp_dist(vec, center);
        if (dist > max_dist) {
            max_dist = dist;
        }
    }

    // Circle boundary of this polygon
    Circle boundary;
    boundary.position = center;
    boundary.radius = max_dist * FixedPoint::fromFloat(1.8f);
    return boundary;
}

void Entity::addHurtbox(const Polygon &hurtbox) {
    vector<Polygon> container;
    container.push_back(hurtbox);
    addHurtbox(container);
}

void Entity::addHurtbox(const vector<Polygon> &hurtbox) {
    vector<Circle> bounds;
    vector<PolyData> poly_data;
    for (const Polygon &poly : hurtbox) {
        bounds.push_back(getBounds(poly));
        poly_data.push_back(PolyData());
    }

    CollisionBox box;
    box.polys = hurtbox;
    box.poly_data = poly_data;
    box.bounds = bounds;
    box.dir = Dir_Left;
    hurtboxes.push_back(box);
}

void Entity::addHitbox(const Polygon &hitbox) {
    vector<Polygon> container;
    container.push_back(hitbox);
    addHitbox(container);
}

void Entity::addHitbox(const vector<Polygon> &hitbox) {
    vector<Circle> bounds;
    vector<PolyData> poly_data;
    for (const Polygon &poly : hitbox) {
        bounds.push_back(getBounds(poly));
        poly_data.push_back(PolyData());
    }

    CollisionBox box;
    box.polys = hitbox;
    box.poly_data = poly_data;
    box.bounds = bounds;
    box.dir = Dir_Left;
    hitboxes.push_back(box);
}
