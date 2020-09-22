#include "GameController.h"
#include "Entity.h"
#include "Player.h"
#include "Chunk.h"
#include "PhysicsEngine.h"
#include "PhysicsController.h"
#include "WaterEntity.h"
#include "CrushBlock.h"
#include "ChunkController.h"
#include "FlowerGenerator.h"

#include <TextureLoader.h>
#include <iostream>
#include <ww_math.h>
#include <StackAllocator.h>
#include <WaywardGL.h>
#include <vector>

using std::cout;
using std::cerr;
using std::endl;
using std::vector;

StackAllocator alloc(1000000000);

const int map_w = 5;
const int map_h = 5;
const int chunk_w = 20;
const int chunk_h = 20;
const int tile_size = 900;

MapDimensions map_dimensions = { map_w, map_h, chunk_w, chunk_h, tile_size };

Flower flower;

GameController::GameController() : physics_controller(this), chunk_controller(this, map_dimensions) {
    physics_controller.setChunkController(&chunk_controller);
    chunk_controller.setPhysicsController(&physics_controller);

    player_input.gc_index = 0;
    Player *player = alloc.allocate<Player>();
    player->data.position.y = FixedPoint::fromInt(4000);
    players.push_back(player);
    addEntity(player);
    
    chunk_controller.generateMap(alloc);

    flower = generateFlower();

    void *ptr = alloc.raw_allocate<CrushBlock>();
    //CrushBlock *crush = new(ptr) CrushBlock(4 * tile_size, 3 * tile_size, tile_size, tile_size);
    //addEntity(crush);
}

GameController::~GameController() {}

static bool fbf_mode = false;
static bool tick_;

void GameController::pretick() {
    player_input.tick();
    tick_ = player_input.isPressed(Button_Other, false) || !fbf_mode;

    if (!tick_) return;
    players[0]->processInput(player_input);
}

void GameController::tick() {
    if (!tick_) return;

    for (Entity *entity : entities) {
        entity->tick();
    }

    // TODO: Multithread this and move it to a better place
    CollisionManifold manifold;
    for (int i = 0; i < water_entities.size(); i++) {
        WaterEntity *e1 = water_entities[i];
        for (int j = i+1; j < water_entities.size(); j++) {
            WaterEntity *e2 = water_entities[j];
            // Run collision checks
            if (PhysicsEngine::checkCollision(e1, e2, &manifold)) {
                Vector2D swap = e1->data.velocity;
                Vector2D pv = manifold.pv;

                FixedPoint mult = FixedPoint::fromFloat(0.9);
                FixedPoint diff = FixedPoint::fromFloat(1.0) - mult;
                e1->data.velocity = mult * e2->data.velocity + diff * swap;
                e2->data.velocity = mult * swap + diff * e2->data.velocity;
                e1->data.position += pv / FixedPoint::fromInt(2);
                e2->data.position -= pv / FixedPoint::fromInt(2);

                e1->collision_count++;
                e2->collision_count++;
            }
        }
    }

    if (reindex_entities || reindex_static_entities) {
        /*chunk_controller.clearChunks(reindex_entities, reindex_static_entities);

        if (reindex_entities) {
            for (Entity *entity : entities) {
                chunk_controller.indexEntity(entity);
            }
        }

        if (reindex_static_entities) {
            for (Entity *entity: static_entities) {
                chunk_controller.indexEntity(entity);
            }
        }*/
        reindex_entities = reindex_static_entities = false;
    }

    physics_controller.runCollisionChecks();
}

void GameController::render() {
    for (Entity *entity : entities) {
        entity->updateSprite();
    }

    WaywardGL::renderSprite(2000, 1000, flower.width, flower.height, flower.texture_handle);
    WaywardGL::render();
}

void GameController::addWaterEntity(WaterEntity *entity) {
    entity->gc = this;
    water_entities.push_back(entity);
    entities.push_back(entity);
}

void GameController::addEntity(Entity *entity) {
    entity->gc = this;
    entity->ID = entity_id;
    entity_id += 1;

    entity->is_static = false;
    chunk_controller.indexEntity(entity);
    entities.push_back(entity);
}

void GameController::addStaticEntity(Entity *entity) {
    entity->gc = this;
    entity->ID = entity_id;
    entity_id += 1;

    entity->is_static = true;
    chunk_controller.indexEntity(entity);
    static_entities.push_back(entity);
}


void GameController::removeEntity(Entity *entity) {
    entity->removeSprite();
    entity->data.hurtbox_handle = -1;
    entity->data.hitbox_handle = -1;

    vector<Entity*> updated_entities;
    if (entity->is_static) {
        static_entities = updated_entities;
        reindex_static_entities = true;
    } else {
        entities = updated_entities;
        reindex_entities = true;
    }
}

StackAllocator *GameController::allocator() {
    return &alloc;
}
