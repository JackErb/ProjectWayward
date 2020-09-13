#include "GameController.h"
#include "Entity.h"
#include "Player.h"
#include "Chunk.h"
#include "PhysicsEngine.h"
#include "PhysicsController.h"
#include "WaterEntity.h"

#include <TextureLoader.h>
#include <iostream>
#include <ww_math.h>
#include <StackAllocator.h>
#include <WaywardGL.h>
#include <vector>
#include "../math/random.h"

using std::cout;
using std::cerr;
using std::endl;
using std::vector;

StackAllocator alloc(10000000);

GameController::GameController() : physics(this) {
    player_input.gc_index = 0;
    Player *player = alloc.allocate<Player>();
    player->data.position.y = FixedPoint::fromInt(4000);
    addEntity(player);

    int size = 750;
    for (int x = 0; x < 40; x++) {
        for (int y = 0; y < 40; y++) {
            int chunkx = (x - 20) * size;
            int chunky = (y - 10) * size;

            if (chunkx <= size * 5 && chunkx >= size * -5
             && chunky > 0 && chunky <= size * 6) {
                continue;
            }


            void *ptr = alloc.raw_allocate<Chunk>();
            Chunk *chunk = new(ptr) Chunk(chunkx, chunky, size, size);
            addEntity(chunk);
        }
    }

    /*for (int x = 0; x < 40; x++) {
        for (int y = 0; y < 70; y++) {
            int n = 800;
            void *ptr = alloc.raw_allocate<Chunk>();
            Chunk *chunk = new(ptr) Chunk((x - 7) * n - 1, -y * n - 1, n + 1, n + 1);
            addEntity(chunk);
        }
    }*/

}

GameController::~GameController() {}

static bool fbf_mode = false;
static bool tick_;

void GameController::pretick() {
    player_input.tick();
    tick_ = player_input.isPressed(Button_Other, false) || !fbf_mode;

    if (!tick_) return;
    entities[0]->processInput(player_input);
}

void GameController::tick() {
    if (!tick_) return;
    for (Entity *entity : entities) {
        entity->tick();
    }

    physics.runCollisionChecks();

    /*
    // TODO: Multithread this and move it to a better place
    Vector2D pv;
    for (int i = 0; i < water_entities.size(); i++) {
        WaterEntity *e1 = water_entities[i];
        for (int j = i+1; j < water_entities.size(); j++) {
            WaterEntity *e2 = water_entities[j];
            // Run collision checks
            if (PhysicsEngine::checkCollision(e1, e2, &pv)) {
                Vector2D swap = e1->data.velocity;

                FixedPoint mult = FixedPoint::fromFloat(0.6);
                FixedPoint diff = FixedPoint::fromFloat(1.0) - mult;
                e1->data.velocity = mult * e2->data.velocity + diff * swap;
                e2->data.velocity = mult * swap + diff * e2->data.velocity;
                e1->data.position += pv / FixedPoint::fromInt(2);
                e2->data.position += -pv / FixedPoint::fromInt(2);

                e1->collision_count++;
                e2->collision_count++;
            }
        }
    }*/
}

void GameController::render() {
    for (Entity *entity : entities) {
        entity->updateSprite();
    }
}

void GameController::addWaterEntity(WaterEntity *entity) {
    entity->gc = this;
    water_entities.push_back(entity);
    entities.push_back(entity);
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
