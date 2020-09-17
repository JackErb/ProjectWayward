#include "GameController.h"
#include "Entity.h"
#include "Player.h"
#include "Chunk.h"
#include "PhysicsEngine.h"
#include "PhysicsController.h"
#include "WaterEntity.h"
#include "CrushBlock.h"
#include "ChunkController.h"

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

StackAllocator alloc(10000000);

const int map_w = 10;
const int map_h = 10;
const int chunk_w = 10;
const int chunk_h = 10;
const int tile_size = 900;

MapDimensions map_dimensions = { map_w, map_h, chunk_w, chunk_h, tile_size };

GameController::GameController() : physics(this), chunks(this, map_dimensions) {
    physics.setChunkController(&chunks);
    chunks.setPhysicsController(&physics);

    player_input.gc_index = 0;
    Player *player = alloc.allocate<Player>();
    player->data.position.y = FixedPoint::fromInt(4000);
    addEntity(player);
    
    chunks.generateMap(alloc);

    void *ptr = alloc.raw_allocate<CrushBlock>();
    CrushBlock *crush = new(ptr) CrushBlock(4 * tile_size, 3 * tile_size, tile_size, tile_size);
    addEntity(crush);
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
    physics.runCollisionChecks();
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
    entity->ID = entity_id;
    entity_id += 1;

    chunks.getChunk(0,0).entities.push_back(entity);
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
