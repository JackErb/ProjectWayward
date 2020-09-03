#include "Player.h"

#include <ww_math.h>
#include <WaywardGL.h>
#include "GroundedState.h"

Player::Player() {
	sprite_handle = WaywardGL::addSprite(1500, 1700);
	texture_id = WaywardGL::loadTexture("jump_3.png");
	WaywardGL::setTexture(texture_id);

	state = new GroundedState(this); 
}

void Player::processInput(const PlayerInput &input) {
	state->processInput(input);
}

void Player::tick() {
	state->tick();

	float x = position.x.toFloat();
	float y = position.y.toFloat();
	WaywardGL::updateSpritePos(sprite_handle, x, y);
}
