#include "AirborneState.h"

#include "Input.h"
#include "Player.h"

#include <iostream>
#include <WaywardGL.h>
#include <SpriteBuffer.h>

using std::cout;
using std::cerr;
using std::endl;

AirborneState::AirborneState(Player *player, AirborneAction action) {
    this->player = player;
    this->data.action = action;
    this->data.frame = 0;
    this->data.accel_x = FixedPoint::ZERO;
}

void AirborneState::switchActionState(AirborneAction action) {
    data.action = action;
    data.frame = 0;
}

void AirborneState::pretick() {
    const PlayerInput *input = player->input;

    switch (data.action) {
      case Airborne_Neutral:
        break;
    }
}

void AirborneState::tick() {
    const PlayerInput *input = player->input;

    switch(data.action) {
      case Airborne_Neutral: {
        if (input->stick.hyp > StickState::DEADZONE) {
            data.accel_x += input->stick.x * attr.airAccel;
            player->data.velocity.x += data.accel_x;
        }
        break;
      }
    }

    FixedPoint vx = player->data.velocity.x;
    if (fp_abs(vx) > attr.maxAirSpeed)
        player->data.velocity.x = fp_sign(vx) * attr.maxAirSpeed;
           
    player->data.position += player->data.velocity;
    player->data.velocity.x *= attr.friction;
    player->data.velocity.y -= attr.gravity;

    data.frame += 1;
}

void AirborneState::switchState(PlayerState *new_state) {
    PlayerState::switchState(new_state);
}


    /*FixedPoint vx = input.stick.x * FixedPoint::fromFloat(50.f);
    player->data.position.x = player->data.position.x + vx;
    FixedPoint vy = input.stick.y * FixedPoint::fromFloat(50.f);
    player->data.position.y = player->data.position.y - vy;*/
