#include "Explosive.h"
#include <WaywardGL.h>
#include "ShapeBuffer.h"
#include <vector>
#include <ww_math.h>
#include "GameController.h"

using std::vector;

Explosive::Explosive(const Vector2D &pos) {
    int w = 800, h = 800;
    float x = pos.x.toFloat(), y = pos.y.toFloat();
    sprite_handle = WaywardGL::shapeBuffer()->addShape(x, y, w, h);
    data.position = pos;

    addHurtbox(poly_square(0, 0, w, h));
    data.hurtbox_handle = 0;

    addHitbox(poly_circle(0, 0, 3000));
    data.hitbox_handle = -1;

    data.hurtbox_bitmask = Bitmask::Stage;
    data.hitbox_bitmask  = Bitmask::None;
    data.bitmask = Bitmask::Explosive;
}

void Explosive::tick() {
    data.frame++;

    if (data.frame == 60) {
        // Explode!
        data.hitbox_handle = 0;
    } else if (data.frame == 61) {
        data.hitbox_handle = -1;
    }


    data.velocity.y -= FixedPoint::fromFloat(4.f);
    data.position += data.velocity;
}

void Explosive::handleCollision(const CollisionManifold &manifold) {
    const Vector2D &pv = manifold.pv;

    data.position += pv;
    if (pv.x == 0 && pv.y > 0) data.velocity.y = 0;
}

void Explosive::handleHit(const CollisionManifold &manifold) {
    gc->removeEntity(manifold.e2);   
}

void Explosive::updateSprite() {
    float x = data.position.x.toFloat();
    float y = data.position.y.toFloat();
    WaywardGL::shapeBuffer()->setShapePos(sprite_handle, x, y);
}

void Explosive::removeSprite() {
    // TODO
}
