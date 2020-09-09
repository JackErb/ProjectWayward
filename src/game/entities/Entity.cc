#include "Entity.h"
#include <WaywardGL.h>
#include "SpriteBuffer.h"

using std::vector;
using std::iterator;

void Entity::updateSprite() {
    float x = data.position.x.toFloat();
    float y = data.position.y.toFloat();
    WaywardGL::spriteBuffer()->setSpritePos(sprite_handle, x, y);
    WaywardGL::spriteBuffer()->setSpriteDir(sprite_handle, data.dir == Dir_Right ? 1 : 0);
}

void flipPolys(CollisionBox *box) {
    for (Polygon &poly : box->polys) {
        if (poly.size() == 2) {
            // Special case for circles
            poly[0].x = -poly[0].x;
            continue;
        }
        for (Vector2D &vec : poly) {
            vec.x = -vec.x;
        }
    }

    for (Circle &bound : box->bounds) {
        bound.position.x = -bound.position.x;
    }
}

const CollisionBox *Entity::polygons_hurt() {
    // TODO: Somehow cache this?
    auto iter = hurtboxes.begin();
    iter += data.hurtbox_handle;

    CollisionBox *box = &(*iter);
    if (data.dir != box->dir) {
        flipPolys(box);
        box->dir = data.dir;
    }
    return box;
}

const CollisionBox *Entity::polygons_hit() {
    // TODO: Somehow cache this?
    auto iter = hitboxes.begin();
    iter += data.hitbox_handle;

    CollisionBox *box = &(*iter);
    if (data.dir != box->dir) {
        flipPolys(box);
        box->dir = data.dir;
    }
    return box;
}

Circle getBounds(const Polygon &poly) {
    Vector2D center = geometric_center(poly);
    
    FixedPoint max_dist = FixedPoint::ZERO;
    for (const Vector2D &vec: poly) {
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
    for (const Polygon &poly : hurtbox) {
        bounds.push_back(getBounds(poly));
    }

    CollisionBox box;
    box.polys = hurtbox;
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
    for (const Polygon &poly : hitbox) {
        bounds.push_back(getBounds(poly));
    }

    CollisionBox box;
    box.polys = hitbox;
    box.bounds = bounds;
    box.dir = Dir_Left;
    hitboxes.push_back(box);
}
