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
}

void GroundedState::switchActionState(GroundedAction action) {
    data.action = action;
    data.frame = 0;
}

void GroundedState::pretick() {
    const PlayerInput *input = player->input;

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
        if (data.frame == 8) {
            player->switchState(new AirborneState(player, Airborne_Neutral));
            player->data.velocity.y = FixedPoint::fromFloat(280.f);
        }
        break;
      case Grounded_Land:
        if (data.frame == 8) {
            switchActionState(Grounded_Dash);
        }
    }

    if (input->isPressed(Button_Jump)) {
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
        break;
      }
      case Grounded_Jumpsquat: {
        int frame = data.frame / 2;
        WaywardGL::spriteBuffer()->setSpriteTexture(player->sprite_handle, frame);
        break;
      }
      case Grounded_Land: {
        int frame = data.frame / 2;
        WaywardGL::spriteBuffer()->setSpriteTexture(player->sprite_handle, 4 + frame);
        break;
      }
    }

    player->data.velocity.x *= attr.friction;
    player->data.velocity.y -= attr.gravity;
    player->data.position += player->data.velocity;
    data.frame += 1;
}

void GroundedState::switchState(PlayerState *new_state) {
    PlayerState::switchState(new_state);
}


    /*FixedPoint vx = input.stick.x * FixedPoint::fromFloat(50.f);
    player->data.position.x = player->data.position.x + vx;
    FixedPoint vy = input.stick.y * FixedPoint::fromFloat(50.f);
    player->data.position.y = player->data.position.y - vy;*/
