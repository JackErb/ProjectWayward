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
    this->data.jumps = 100;
    this->data.fastfall = false;
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
    FixedPoint hyp = input->stick.hyp;

    if (input->stick.inDir(StickState::Down)) {
        data.fastfall = true;
    }

    switch(data.action) {
      case Airborne_Neutral: {
        if (hyp > StickState::DEADZONE) {
            FixedPoint accel_x = input->stick.x * attr.airAccel;
            player->data.velocity.x += accel_x;   
        }
        break;
      }
    }


    // Vectorand cap air speed
    FixedPoint vx = player->data.velocity.x;
    if (fp_abs(vx) > attr.maxAirSpeed) {
        player->data.velocity.x = fp_sign(vx) * attr.maxAirSpeed;
    }

    // Double jump
    if (data.jumps > 0 && input->isPressed(Button_Jump, false)) {
        data.jumps--;
        player->data.velocity.y = attr.doubleJump;
    }
    
    // Apply gravity and limit fall speed
    player->data.velocity.y -= attr.gravity;
    FixedPoint maxFall = data.fastfall ? attr.maxFastFall : attr.maxFall;
    if (player->data.velocity.y < maxFall) {
        player->data.velocity.y = maxFall;
    }

    player->data.position += player->data.velocity;
    // Only apply friction if player is not vectoring 
    if (hyp <= StickState::DEADZONE) player->data.velocity.x *= attr.airFriction;

    data.frame += 1;
}

void AirborneState::handleCollision(Entity *entity, const Vector2D &pv) {

}

void AirborneState::switchState(PlayerState *new_state) {
    PlayerState::switchState(new_state);
}


    /*FixedPoint vx = input.stick.x * FixedPoint::fromFloat(50.f);
    player->data.position.x = player->data.position.x + vx;
    FixedPoint vy = input.stick.y * FixedPoint::fromFloat(50.f);
    player->data.position.y = player->data.position.y - vy;*/
