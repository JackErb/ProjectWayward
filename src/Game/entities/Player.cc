#include "Player.h"

#include <ww_math.h>
#include <WaywardGL.h>
#include "GroundedState.h"
#include "SpriteBuffer.h"

using std::vector;

Player::Player() {
    sprite_handle = WaywardGL::spriteBuffer()->addSprite(0, 0, 1500, 1700, 0);

    vector<Polygon> polygons;
    polygons.push_back(poly_square(0, 0, 1500, 1700));
    hurtboxes.push_back(polygons);
    data.hurtbox_handle = 0;

    state = new GroundedState(this); 
}

void Player::processInput(const PlayerInput &input) {
    state->processInput(input);
}

void Player::tick() {
    state->tick();

    float x = data.position.x.toFloat();
    float y = data.position.y.toFloat();
    WaywardGL::spriteBuffer()->setSpritePos(sprite_handle, x, y);

    frame++;
    if (frame % 5 == 0) {
        WaywardGL::spriteBuffer()->setSpriteTexture(sprite_handle, test);
        test = 1 - test;
    }
}

void Player::handleCollision(Entity *e, const Vector2D &pv) {
    data.position = data.position + pv;
}
