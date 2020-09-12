#include "GameController.h"
#include "Entity.h"
#include "Player.h"
#include "Chunk.h"
#include "PhysicsEngine.h"
#include "PhysicsMultithreader.h"
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
using PhysicsMultithreader::JobReturn;

StackAllocator alloc(10000000);

GameController::GameController() {
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

    PhysicsMultithreader::init();
}

GameController::~GameController() {
    PhysicsMultithreader::shutdown();
}

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
    }

    PhysicsMultithreader::runJobs(entities, this);
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

vector<JobReturn> GameController::runCollisionChecks(Entity *e1) {
    Vector2D pv;
    vector<JobReturn> collisions;

    /* Hurtbox collisions */
    int hurtbox_mask = e1->data.hurtbox_bitmask;
    if (hurtbox_mask != 0 && e1->data.hurtbox_handle != -1) {
        for (Entity *e2 : entities) {
            if (e1 == e2) continue;
            int bitmask = e2->data.bitmask;
            if (hurtbox_mask & bitmask) {
                bool collision = PhysicsEngine::checkCollision(e1, e2, &pv);
                if (!collision) continue;

                JobReturn ret = {e1, e2, 0, hurtbox_mask & bitmask, pv};
                collisions.push_back(ret);
            }
        }
    }

    int hitbox_mask = e1->data.hitbox_bitmask;
    if (hitbox_mask != 0 && e1->data.hitbox_handle != -1) {
        for (Entity *e2 : entities) {
            if (e1 == e2) continue;
            int bitmask = e2->data.bitmask;
            if (hitbox_mask & bitmask) {
                bool collision = PhysicsEngine::checkHitboxCollision(e1, e2, &pv);
                if (!collision) continue;

                JobReturn ret = {e1, e2, 1, hitbox_mask & bitmask, pv};
                collisions.push_back(ret);
            }
        }
    }
    return collisions;
}

StackAllocator *GameController::allocator() {
    return &alloc;
}
