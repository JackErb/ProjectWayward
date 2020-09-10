#include "GameController.h"
#include "Entity.h"
#include "Player.h"
#include "Chunk.h"
#include "PhysicsEngine.h"
#include "PhysicsMultithreader.h"

#include <TextureLoader.h>
#include <iostream>
#include <ww_math.h>
#include <StackAllocator.h>
#include <WaywardGL.h>
#include <vector>
#include <MetaballBuffer.h>
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

    int size = 800;
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

    Random r(100);
    for (int i = 0; i < num_water; i++) {
        WaterBall water;
        water.handle = WaywardGL::waterBuffer()->addMetaball(0, 0, r.rand(700,1300));
        water.vel.x = FixedPoint::fromInt(r.rand(-60, 60));
        water.vel.y = FixedPoint::fromInt(r.rand(-60, 60));
        this->water[i] = water;
    }

    PhysicsMultithreader::init();
}

GameController::~GameController() {

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

    for (int i = 0; i < num_water; i++) {
        WaterBall &water = this->water[i];
        water.pos += water.vel;
        if (fp_abs(water.pos.x) > FixedPoint::fromInt(8000)) {
            water.vel.x = -water.vel.x;
        }
        if (fp_abs(water.pos.y) > FixedPoint::fromInt(8000)) {
            water.vel.y = -water.vel.y;
        }
        WaywardGL::waterBuffer()->setMetaballPos(water.handle, water.pos.x.toFloat(), water.pos.y.toFloat());
    }
    
    PhysicsMultithreader::runJobs(entities, this);
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

                JobReturn ret = {e1, e2, 0, pv};
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

                JobReturn ret = {e1, e2, 1, pv};
                collisions.push_back(ret);
            }
        }
    }
    return collisions;
}

StackAllocator *GameController::allocator() {
    return &alloc;
}
