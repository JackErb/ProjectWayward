#include "GameController.h"
#include "Entity.h"
#include "Player.h"
#include "Chunk.h"
#include "PhysicsEngine.h"

#include <TextureLoader.h>
#include <iostream>
#include <ww_math.h>

using std::cout;
using std::cerr;
using std::endl;

GameController::GameController() {
	player_input.gc_index = 0;
	entities.push_back(new Player());
	entities.push_back(new Chunk());
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

	for (int i = 0; i < entities.size()-1; i++) {
		Entity *e1 = entities[i];
		for (int j = i+1; j < entities.size(); j++) {
			Entity *e2 = entities[j];

			Vector2D pv;
			bool collision = PhysicsEngine::checkCollision(e1, e2, &pv);
			if (true) {
				e1->handleCollision(e2, pv);
				e2->handleCollision(e1, -pv);
			}
		}
	}
}

void GameController::render() {

}
