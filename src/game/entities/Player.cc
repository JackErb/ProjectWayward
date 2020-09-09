#include "Player.h"

#include <ww_math.h>
#include <WaywardGL.h>
#include "GroundedState.h"
#include "SpriteBuffer.h"
#include <iostream>
#include "GameController.h"
#include "Explosive.h"
#include <StackAllocator.h>

using std::cout;
using std::cerr;
using std::endl;

using std::vector;

Player::Player() {
    sprite_handle = WaywardGL::spriteBuffer()->addSprite(0, 0, 1500, 1700, 0);

    vector<Polygon> polygons;
    polygons.push_back(poly_square(0, 0, 1000, 1600));
    hurtboxes.push_back(polygons);
    data.hurtbox_handle = 0;

    data.hitbox_handle = -1;

    data.hurtbox_bitmask |= Bitmask::Stage;
    data.bitmask = Bitmask::Player;

    state = new GroundedState(this, Grounded_Neutral); 
}

void Player::processInput(const PlayerInput &input) {
    this->input = &input;
    
    if (input.stick.hyp > StickState::DEADZONE && state->type() == State_Grounded) {
        data.dir = input.stick.x >= 0 ? Dir_Right : Dir_Left;
    }

    if (input.isPressed(Button_Attack, false)) {
        void *ptr = gc->allocator()->raw_allocate<Explosive>();
        Explosive *explosive = new(ptr) Explosive(data.position);
        gc->addEntity(explosive);
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
            data.velocity.x = 0;
            GroundedState *landing_lag = new GroundedState(this, Grounded_Land);
            state->switchState(landing_lag);
        }
    }

    state->handleCollision(e, pv);
}

void Player::handleHit(Entity *e, const Vector2D &pv) {

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

void Player::removeSprite() {
    WaywardGL::spriteBuffer()->removeSprite(sprite_handle);
}
