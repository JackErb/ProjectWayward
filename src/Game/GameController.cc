#include "GameController.h"
#include "Entity.h"
#include "Player.h"

#include <TextureLoader.h>

GameController::GameController() {
	player_input.gc_index = 0;
	entities.push_back(new Player());
}

GameController::~GameController() {

}


void GameController::pretick() {
	player_input.tick();
	entities[0]->processInput(player_input);
}

void GameController::tick() {
	for (Entity *entity : entities) {
		entity->tick();
	}
}

void GameController::render() {

}
