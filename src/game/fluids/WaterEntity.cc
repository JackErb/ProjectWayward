#include "WaterEntity.h"
#include <WaywardGL.h>
#include <MetaballBuffer.h>
#include <iostream>

using std::cout;
using std::cerr;
using std::endl;

WaterEntity::WaterEntity(int x, int y, int r) {
    sprite_handle = WaywardGL::waterBuffer()->addMetaball(x, y, r);

    data.frame = 0;
    data.bitmask = Bitmask::Water;

    data.hitbox_handle = -1;
    data.hitbox_bitmask = Bitmask::None;

    this->r = r;
    addHurtbox(poly_circle(0, 0, r));
    data.hurtbox_handle = 0;
    data.hurtbox_bitmask = Bitmask::Stage | Bitmask::Player;
}

WaterEntity::~WaterEntity() {

}

void WaterEntity::tick() {
    //data.velocity.y = fp_sign(data.velocity.y) * fp_min(data.velocity.y, FixedPoint::fromInt(100));
    if (apply_gravity)
        data.velocity.y -= FixedPoint::fromFloat(4.f);
    data.position += data.velocity;

    data.frame++;

    if (data.frame % 30 == 0) {
        collision_count /= 30;
        FixedPoint radius = FixedPoint::fromFloat(r + collision_count * 80.f);
        hurtboxes[0].polys[0][1].x = radius;
        hurtboxes[0].bounds[0].radius = radius;
        collision_count = 0;
    }

    apply_gravity = true;
}

void WaterEntity::handleCollision(Entity *entity, const Vector2D &pv, int bitmask) {
    if (bitmask & Bitmask::Stage) {
        data.position += pv;
        data.velocity = FixedPoint::fromFloat(-0.3) * data.velocity;
        if (pv.x == FixedPoint::ZERO) apply_gravity = false;
    } else if (bitmask & Bitmask::Player) {
        Vector2D vel = unit_vec(pv - entity->data.velocity);
        data.velocity += unit_vec(vel) * FixedPoint::fromFloat(10);
    }
}

void WaterEntity::updateSprite() {
    float x = data.position.x.toFloat(), y = data.position.y.toFloat();
    float r = hurtboxes[0].polys[0][1].x.toFloat();
    WaywardGL::waterBuffer()->setMetaballPos(sprite_handle, x, y);
    WaywardGL::waterBuffer()->setMetaballRadius(sprite_handle, r);
}

void WaterEntity::removeSprite() {
    cerr << "Method not implemented." << endl;
    exit(1);
}
