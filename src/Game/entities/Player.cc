#include "Player.h"

#include <ww_math.h>
#include <WaywardGL.h>
#include "GroundedState.h"
#include "SpriteBuffer.h"
#include <iostream>

using std::cout;
using std::cerr;
using std::endl;

using std::vector;

Player::Player() {
    sprite_handle = WaywardGL::spriteBuffer()->addSprite(0, 0, 1500, 1700, 0);

    vector<Polygon> polygons;
    polygons.push_back(poly_square(0, 0, 1500, 1700));
    hurtboxes.push_back(polygons);
    data.hurtbox_handle = 0;

    state = new GroundedState(this, Grounded_Neutral); 
}

void Player::processInput(const PlayerInput &input) {
    this->input = &input;
    
    if (input.stick.hyp > StickState::DEADZONE) {
        data.dir = input.stick.x >= 0 ? Dir_Right : Dir_Left;
    }

    state->pretick();
}

void Player::tick() {
    state->tick();
}

void Player::handleCollision(Entity *e, const Vector2D &pv) {
    data.position += pv;
    if (pv.x == 0 && pv.y > 0) {
        data.velocity.y = 0;
        if (state->type() == State_Airborne) {
            GroundedState *landing_lag = new GroundedState(this, Grounded_Land);
            state->switchState(landing_lag);
        }
    }
}

void Player::switchState(PlayerState *new_state) {
    delete state;
    state = new_state;
}

void Player::updateSprite() {
    float x = data.position.x.toFloat();
    float y = data.position.y.toFloat();
    WaywardGL::spriteBuffer()->setSpritePos(sprite_handle, x, y);
    WaywardGL::spriteBuffer()->setSpriteDir(sprite_handle, data.dir == Dir_Right ? 1 : 0);
}
