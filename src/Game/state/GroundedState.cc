#include "GroundedState.h"

#include "Input.h"
#include "Player.h"

#include <iostream>

void GroundedState::processInput(const PlayerInput &input) {
	FixedPoint vel = input.stick.x * FixedPoint::fromFloat(10.f);
	player->position.x = player->position.x + vel;
}

void GroundedState::tick() {

}
