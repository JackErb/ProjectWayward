#include "GameController.h"
#include "Entity.h"
#include "Player.h"
#include "Chunk.h"
#include "PhysicsEngine.h"

#include <TextureLoader.h>
#include <iostream>
#include <ww_math.h>
#include <ww_memory.h>
#include <ww_generator.h>
#include <WaywardGL.h>
#include <vector>

using std::cout;
using std::cerr;
using std::endl;
using std::vector;

StackAllocator alloc(100000);

GameController::GameController() {
    player_input.gc_index = 0;
    Player *player = alloc.allocate<Player>();
    player->data.position.y = FixedPoint::fromInt(4000);
    entities.push_back(player);

    for (int x = 0; x < 30; x++) {
        for (int y = 0; y < 30; y++) {
            int n = 800;
            void *ptr = alloc.raw_allocate<Player>();
            Chunk *chunk = new(ptr) Chunk((x - 7) * n - 1, -y * n - 1, n + 1, n + 1);
            stage_entities.push_back(chunk);
        }
    }

    /*GenOptions::init();
    GeneratorOptions opt = GenOptions::TestCaveGen;
    opt.mapWidth = 6;
    opt.mapHeight = 5;
    LevelData level_data = WWGenerator::generateLevel(opt);
    for (vector<ChunkData> chunk_row : level_data.chunks) {
        for (ChunkData chunk_data : chunk_row) {
            entities.push_back(new Chunk(chunk_data));           
        }
    }*/
}

GameController::~GameController() {

}

static bool tick_ = true;

void GameController::pretick() {
    player_input.tick();
    tick_ = player_input.isPressed(Button_Attack, false);
    tick_ = tick_ || player_input.isPressed(Button_Other);
    tick_ = true;

    if (!tick_) return;
    entities[0]->processInput(player_input);
}

void GameController::tick() {
    if (!tick_) return;
    for (Entity *entity : entities) {
        entity->tick();
    }

    for (Entity *e1 : entities) {
        for (Entity *e2 : stage_entities) {
            Vector2D pv;
            bool collision = PhysicsEngine::checkCollision(e1, e2, &pv);
            if (collision) {
                e1->handleCollision(e2, pv);
                e2->handleCollision(e1, -pv);
            }
        }
    }
}

void GameController::render() {
    for (Entity *entity : entities) {
        entity->updateSprite();
    }

    for (Entity *entity : stage_entities) {
        entity->updateSprite();
    }
}
