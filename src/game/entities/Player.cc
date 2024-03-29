#include "Player.h"

#include <ww_math.h>
#include <WaywardGL.h>
#include "GroundedState.h"
#include "AirborneState.h"
#include "SpriteBuffer.h"
#include <iostream>
#include "GameController.h"
#include "Explosive.h"
#include <StackAllocator.h>
#include <WaterEntity.h>
#include <Input.h>

using std::cout;
using std::cerr;
using std::endl;

using std::vector;

Player::Player() {
    sprite_handle = WaywardGL::spriteBuffer()->addSprite(0, 0, 1500, 1700, 0);

    addHurtbox(poly_square(0, 0, 1000, 1500));
    data.hurtbox_handle = 0;

    addHitbox(poly_square(-900, 200, 600, 600));
    data.hitbox_handle = -1;
    data.hitbox_bitmask = Bitmask::Stage;

    data.hurtbox_bitmask = Bitmask::Stage;
    data.bitmask = Bitmask::Player;

    state = new AirborneState(this, Airborne_Neutral);
}

void Player::processInput(const PlayerInput &input) {
    this->input = &input;
    
    if (input.stick.hyp > StickState::DEADZONE && state->type() == State_Grounded) {
        data.dir = input.stick.x >= 0 ? Dir_Right : Dir_Left;
    }

    if (input.isPressed(Button_Attack, false)) {
        if (input.stick.inDir(Down)) {
            void *ptr = gc->allocator()->raw_allocate<Explosive>();
            Explosive *explosive = new(ptr) Explosive(data.position);
            gc->addEntity(explosive);
        } else {
            data.hitbox_handle = 0;
        }
    } else {
        data.hitbox_handle = -1;
    }

    if (input.isPressed(Button_Other, false)) {
        void *ptr = gc->allocator()->raw_allocate<WaterEntity>();
        WaterEntity *water = new(ptr) WaterEntity(0, 0, 290);
        water->data.position = data.position;
        water->data.velocity = data.velocity;
        gc->addWaterEntity(water);
    }

    state->pretick();
}

void Player::tick() {
    state->tick();
}

void Player::handleCollision(const CollisionManifold &manifold) {
    const Vector2D &pv = manifold.pv;

    data.position += pv;
    if (pv.x == 0 && pv.y > 0 && data.velocity.y < 0) {
        data.velocity.y = 0;
        if (state->type() == State_Airborne) {
            data.velocity.x = 0;
            GroundedState *landing_lag = new GroundedState(this, Grounded_Land);
            state->switchState(landing_lag);
        }
    }

    if (pv.x == 0 && pv.y < 0) {
        // head bonk on the ceiling
        data.velocity.y /= FixedPoint::fromInt(2);
    }

    state->handleCollision(manifold);
}

void Player::handleHit(const CollisionManifold &manifold) {
    if (manifold.mask & Bitmask::Stage) {
        manifold.e1->data.velocity.x = FixedPoint::fromFloat(-200.f);
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

void Player::removeSprite() {
    WaywardGL::spriteBuffer()->removeSprite(sprite_handle);
}
