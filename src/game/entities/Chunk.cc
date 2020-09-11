#include "Chunk.h"
#include <WaywardGL.h>
#include <ww_math.h>
#include "ShapeBuffer.h"
#include "SpriteBuffer.h"
#include "GameController.h"
#include <cstdlib>

using std::vector;
using std::cout;
using std::endl;

Chunk::Chunk(int x, int y, int w, int h) {
    data.position = Vector2D(FixedPoint::fromInt(x), FixedPoint::fromInt(y));

    int texture = rand() % 3;
    sprite_handle = WaywardGL::tileBuffer()->addSprite(x, y, w, h, texture);
   
    addHurtbox(poly_square(0,0,w,h));
    data.hurtbox_handle = 0;
    data.hurtbox_bitmask = Bitmask::None;

    data.hitbox_handle = -1;
    data.hitbox_bitmask = Bitmask::None;
    data.bitmask = Bitmask::Stage;
}

void Chunk::tick() {

}

void Chunk::handleCollision(Entity *e, const Vector2D &pv, int bitmask) {
    // data.velocity.y = FixedPoint::fromFloat(40.f);
}

void Chunk::handleHit(Entity *e, const Vector2D &pv, int bitmask) {
    gc->removeEntity(this);
}

void Chunk::updateSprite() {
    Vector2D pos = data.position;
    float x = pos.x.toFloat(), y = pos.y.toFloat();
    WaywardGL::tileBuffer()->setSpritePos(sprite_handle, x, y);
}

void Chunk::removeSprite() {
    WaywardGL::tileBuffer()->removeSprite(sprite_handle);
    sprite_handle = -1;
}
