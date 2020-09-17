#include "GroundedState.h"

#include "Input.h"
#include "Player.h"

#include <iostream>
#include <WaywardGL.h>
#include <SpriteBuffer.h>
#include "AirborneState.h"

using std::cout;
using std::cerr;
using std::endl;

GroundedState::GroundedState(Player *player, GroundedAction action) {
    this->player = player;
    this->data.action = action;
    this->data.frame = 0;
    this->data.shorthop = false;
}

void GroundedState::switchActionState(GroundedAction action) {
    data.action = action;
    data.frame = 0;
}

void GroundedState::pretick() {
    const PlayerInput *input = player->input;

    if (!data.grounded) {
        player->switchState(new AirborneState(player, Airborne_Neutral));
        return;
    }
    data.grounded = false;

    switch (data.action) {
      case Grounded_Neutral:
        if (input->stick.hyp > StickState::DEADZONE) {
            switchActionState(Grounded_Dash);
        }
        break;
      case Grounded_Dash:
        if (input->stick.hyp <= StickState::DEADZONE) {
            switchActionState(Grounded_Neutral);
        }
        break;
      case Grounded_Jumpsquat:
        if (input->isReleased(Button_Jump)) data.shorthop = true;
        if (data.frame == 5) {
            int dir = 0;
            if (input->stick.inDir(Left)) dir = -1;
            if (input->stick.inDir(Right)) dir = 1;

            FixedPoint vx = dir * attr.maxAirSpeed;
            FixedPoint vy = data.shorthop ? attr.shortJump : attr.fullJump;
            player->data.velocity = {vx, vy};
            player->switchState(new AirborneState(player, Airborne_Neutral));
            return;
        }
        break;
      case Grounded_Land:
        if (data.frame == 5) {
            switchActionState(Grounded_Neutral);
        }
    }

    if (input->isPressed(Button_Jump) && data.action != Grounded_Jumpsquat) {
        player->data.velocity.x = 0;
        switchActionState(Grounded_Jumpsquat);
    }
}

void GroundedState::tick() {
    const PlayerInput *input = player->input;

    switch(data.action) {
      case Grounded_Neutral: {
        break;
      }
      case Grounded_Dash: {
        FixedPoint vx = input->stick.x;
        player->data.velocity.x = vx * attr.dashSpeed;
        WaywardGL::spriteBuffer()->setSpriteTexture(player->sprite_handle, 8 + (data.frame / 5)  % 10);
        break;
      }
      case Grounded_Jumpsquat: {
        int frame = data.frame;
        WaywardGL::spriteBuffer()->setSpriteTexture(player->sprite_handle, frame / 2);
        break;
      }
      case Grounded_Land: {
        int frame = data.frame;
        WaywardGL::spriteBuffer()->setSpriteTexture(player->sprite_handle, 4 + frame / 2);
        break;
      }
    }

    player->data.velocity.x *= attr.groundFriction;
    player->data.velocity.y -= attr.gravity;
    player->data.position += player->data.velocity;
    data.frame += 1;
}

void GroundedState::handleCollision(const CollisionManifold &manifold) {
    const Vector2D &pv = manifold.pv;

    if (pv.x == 0 && pv.y > 0) {
        data.grounded = true;       
    }
}

void GroundedState::switchState(PlayerState *new_state) {
    PlayerState::switchState(new_state);
}


    /*FixedPoint vx = input.stick.x * FixedPoint::fromFloat(50.f);
    player->data.position.x = player->data.position.x + vx;
    FixedPoint vy = input.stick.y * FixedPoint::fromFloat(50.f);
    player->data.position.y = player->data.position.y - vy;*/
