#include "GroundedState.h"

#include "Input.h"
#include "Player.h"

#include <iostream>

void GroundedState::processInput(const PlayerInput &input) {
	FixedPoint vx = input.stick.x * FixedPoint::fromFloat(10.f);
	player->data.position.x = player->data.position.x + vx;

	
	FixedPoint vy = input.stick.y * FixedPoint::fromFloat(10.f);
	player->data.position.y = player->data.position.y - vy;
}

void GroundedState::tick() {

}



void DashState::processInput(const PlayerInput &input) {
	
}

void DashState::tick() {
	
}



void LandingLagState::processInput(const PlayerInput &input) {

}

void LandingLagState::tick() {

}
