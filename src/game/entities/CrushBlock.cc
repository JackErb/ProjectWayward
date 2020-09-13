#include "CrushBlock.h"
#include <WaywardGL.h>
#include <SpriteBuffer.h>

CrushBlock::CrushBlock(int x, int y, int w, int h) {
    data.position = Vector2D(FixedPoint::fromInt(x), FixedPoint::fromInt(y));

    int texture = 4;
    sprite_handle = WaywardGL::tileBuffer()->addSprite(x, y, w, h, texture);
   
    addHurtbox(poly_square(0,0,w,h));
    data.hurtbox_handle = 0;
    data.hurtbox_bitmask = Bitmask::Chunk;

    data.hitbox_handle = -1;
    data.hitbox_bitmask = Bitmask::None;
    data.bitmask = Bitmask::Stage;
}

CrushBlock::~CrushBlock() {

}

void CrushBlock::tick() {

}

void CrushBlock::handleCollision(Entity *entity, const Vector2D &pv, int bitmask) {
    data.velocity = Vector2D();
    data.position += pv;
}

void CrushBlock::updateSprite() {
    float x = data.position.x.toFloat(), y = data.position.y.toFloat();
    WaywardGL::tileBuffer()->setSpritePos(sprite_handle, x, y);
}

void CrushBlock::removeSprite() {
    WaywardGL::tileBuffer()->removeSprite(sprite_handle);
}
