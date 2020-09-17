#include "CrushBlock.h"
#include "PhysicsTypes.h"
#include <WaywardGL.h>
#include <SpriteBuffer.h>

CrushBlock::CrushBlock(int x, int y, int w, int h) {
    data.position = Vector2D(FixedPoint::fromInt(x), FixedPoint::fromInt(y));

    int texture = 4;
    sprite_handle = WaywardGL::tileBuffer()->addSprite(x, y, w, h, texture);
   
    addHurtbox({poly_square(0,0,w,h)});
    addHitbox({poly_square(w*-3,0,w*-6,h)});

    data.hurtbox_handle = 0;
    data.hurtbox_bitmask = Bitmask::Stage;

    data.hitbox_handle = 0;
    data.hitbox_bitmask = Bitmask::None;
    data.bitmask = Bitmask::Stage;
}

CrushBlock::~CrushBlock() {

}

void CrushBlock::tick() {
    data.position += data.velocity;
}

void CrushBlock::handleCollision(const CollisionManifold &manifold) {
    if (manifold.mask & Bitmask::Stage) {
        data.velocity = Vector2D();
        data.position += manifold.pv;
    }
}

void CrushBlock::handleHit(const CollisionManifold &manifold) {

}

void CrushBlock::updateSprite() {
    float x = data.position.x.toFloat(), y = data.position.y.toFloat();
    WaywardGL::tileBuffer()->setSpritePos(sprite_handle, x, y);
}

void CrushBlock::removeSprite() {
    WaywardGL::tileBuffer()->removeSprite(sprite_handle);
}
