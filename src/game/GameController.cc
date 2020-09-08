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
    addEntity(player);

    for (int x = 0; x < 20; x++) {
        for (int y = 0; y < 20; y++) {
            int n = 800;
            void *ptr = alloc.raw_allocate<Chunk>();
            Chunk *chunk = new(ptr) Chunk((x - 7) * n - 1, -y * n - 1, n + 1, n + 1);
            addEntity(chunk);
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

    Vector2D pv;
    for (Entity *e1 : entities) {
        /* Hurtbox collisions */
        int hurtbox_mask = e1->data.hurtbox_bitmask;
        if (!hurtbox_mask || e1->data.hurtbox_handle == -1) goto hitbox_check;
        for (Entity *e2 : entities) {
            if (e1 == e2) continue;
            int bitmask = e2->data.bitmask;
            if (hurtbox_mask & bitmask) {
                bool collision = PhysicsEngine::checkCollision(e1, e2, &pv);
                if (!collision) continue;
                e1->handleCollision(e2, pv);
                e2->handleCollision(e1, -pv);
            }
        }

      hitbox_check:
        int hitbox_mask = e1->data.hitbox_bitmask;
        if (!hitbox_mask || e1->data.hitbox_handle == -1) continue;
        for (Entity *e2 : entities) {
            if (e1 == e2) continue;
            int bitmask = e2->data.bitmask;
            if (hitbox_mask & bitmask) {
                bool collision = PhysicsEngine::checkHitboxCollision(e1, e2, &pv);
                if (!collision) continue;
                e2->handleHit(e1, pv);
            }
        }
    }
}

void GameController::render() {
    for (Entity *entity : entities) {
        entity->updateSprite();
    }
}

void GameController::addEntity(Entity *entity) {
    entity->gc = this;
    entities.push_back(entity);
}

void GameController::removeEntity(Entity *entity) {
    entity->removeSprite();

    vector<Entity*> updated_entities;
    for (Entity *e : entities) {
        if (e != entity) updated_entities.push_back(e);
    }
    entities = updated_entities;
}

StackAllocator *GameController::allocator() {
    return &alloc;
}
