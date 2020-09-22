#include "GameController.h"
#include "ChunkController.h"
#include "PhysicsController.h"
#include "StackAllocator.h"
#include "PhysicsTypes.h"

#include "Chunk.h"
#include <iostream>

using std::vector;
using std::cout;
using std::cerr;
using std::endl;
using std::unique_lock;
using std::mutex;
using std::pair;

ChunkController::ChunkController(GameController *gc, const MapDimensions &dim) : game_controller(gc), dimensions(dim) {
    // Initialize chunk containers
    chunks = new ChunkContainer*[dimensions.map_width];
    for (int x = 0; x < dimensions.map_width; x++) {
        chunks[x] = new ChunkContainer[dimensions.map_height];
    }

    chunk_size = { FixedPoint::fromInt(dim.chunk_width * dim.tile_size),
                   FixedPoint::fromInt(dim.chunk_height * dim.tile_size) };
}

void ChunkController::generateMap(StackAllocator &alloc) {
    int map_w = dimensions.map_width;
    int map_h = dimensions.map_height;
    int chunk_w = dimensions.chunk_width;
    int chunk_h = dimensions.chunk_height;
    int tile_size = dimensions.tile_size;

    // Fill in the map
    for (int x = 0; x < map_w; x++) {
        for (int y = 0; y < map_h; y++) {
            ChunkContainer &chunk_container =  chunks[x][y];
            int chunk_x = (x - map_w/2) * chunk_w * tile_size;
            int chunk_y = (y - map_h/2) * chunk_h * tile_size;

            // Bottom left corner of chunk
            chunk_container.position = { FixedPoint::fromInt(chunk_x), FixedPoint::fromInt(chunk_y) };

            // Make boundary circle centered in chunk and have a radius encompassing the chunk
            chunk_container.boundary.position = chunk_container.position + chunk_size / FixedPoint::fromInt(2);
            chunk_container.boundary.radius = fp_dist(chunk_container.position,
                                                      chunk_container.boundary.position) * FixedPoint::fromFloat(1.1f);

            if (x == map_w / 2 && y == map_h / 2) continue;
            for (int tx = 0; tx < chunk_w; tx++) {
                for (int ty = 0; ty < chunk_h; ty++) {
                    int tile_x = chunk_x + tx * tile_size + tile_size / 2;
                    int tile_y = chunk_y + ty * tile_size + tile_size / 2;

                    void *ptr = alloc.raw_allocate<Chunk>();
                    Chunk *chunk = new (ptr) Chunk(tile_x, tile_y, tile_size, tile_size);
                    chunk->updateSprite();
                    chunk_container.static_entities.push_back(chunk);
                    game_controller->addStaticEntity(chunk);
                }
            }
        }
    }

    // Center of origin chunk
    origin = chunks[0][0].position;
}

ChunkController::~ChunkController() {

}

void ChunkController::setPhysicsController(PhysicsController *pc) {
    physics_controller = pc;
}

void ChunkController::resetFrame() {
    for (int x = 0; x < dimensions.map_width; x++) {
        for (int y = 0; y < dimensions.map_height; y++) {
            chunks[x][y].other_entities.clear();
        }
    }
}

bool inMap(const MapDimensions &dim, int x, int y) {
    return x >= 0 && x < dim.map_width && y >= 0 && y < dim.map_height;
}

void ChunkController::checkAdjacentChunks(Entity *entity, int x, int y) {
    const vector<Circle> &hurtbox = entity->polygons_hurt().bounds;
    const vector<Circle> &hitbox = entity->polygons_hit().bounds;
    const Vector2D &position = entity->data.position;

    // Check if colliding with adjacent chunks
    for (int i = 0; i < 9; i++) {
        if (i == 4) continue;
        int chunk_x = x + (i % 3 - 1);
        int chunk_y = y + (i / 3 - 1);

        if (inMap(dimensions, chunk_x, chunk_y)) {
            const Circle &chunk_boundary = chunks[chunk_x][chunk_y].boundary;
            for (const Circle &c : hurtbox) {
                FixedPoint dist = fp_distsqr(chunk_boundary.position, position + c.position);
                if (dist <= chunk_boundary.radius * chunk_boundary.radius + c.radius * c.radius) {
                    unique_lock<mutex> lock(chunks[chunk_x][chunk_y].chunk_mutex);
                    chunks[chunk_x][chunk_y].other_entities.insert(entity);
                }
            }

            for (const Circle &c : hitbox) {
                FixedPoint dist = fp_distsqr(chunk_boundary.position, position + c.position);
                if (dist <= chunk_boundary.radius * chunk_boundary.radius + c.radius * c.radius) {
                    unique_lock<mutex> lock(chunks[chunk_x][chunk_y].chunk_mutex);
                    chunks[chunk_x][chunk_y].other_entities.insert(entity);
                }
            }
        }
    }
}

pair<int, int> ChunkController::getChunk(Entity *entity) {
    Vector2D position = (entity->data.position - origin) / chunk_size;
    return { position.x.toInt(), position.y.toInt() };
}

void ChunkController::updatePartitionForChunk(int x, int y) {
    if (!inMap(dimensions, x, y)) return;

    ChunkContainer &chunk = chunks[x][y];
    auto it = chunk.entities.begin();
    while (it != chunk.entities.end()) {
        Entity *entity = *it;

        pair<int, int> new_chunk = getChunk(entity); 
        int new_x = new_chunk.first;
        int new_y = new_chunk.second;

        bool has_moved_chunks = x != new_x || y != new_y;
        if (has_moved_chunks) {
            // Remove from current chunk.
            {
                unique_lock<mutex> lock(chunk.chunk_mutex);
                it = chunk.entities.erase(it);
            }

            // Add to new chunk
            if (inMap(dimensions, new_x, new_y)) {
                ChunkContainer &new_chunk = chunks[new_x][new_y];
                unique_lock<mutex> lock(new_chunk.chunk_mutex);
                new_chunk.entities.push_back(entity);
            }
        } else {
            it++;
        }

        if (!inMap(dimensions, new_x, new_y)) {
            // Out of bounds
            entity->data.chunk_x = -1;
            entity->data.chunk_y = -1;
        }

        checkAdjacentChunks(entity, new_x, new_y);
    }
}

void ChunkController::clearChunks(bool clear_dynamic, bool clear_static) {
    for (int x = 0; x < dimensions.map_width; x++) {
        for (int y = 0; y < dimensions.map_height; y++) {
            ChunkContainer &chunk = chunks[x][y];

            if (clear_dynamic) {
                chunk.entities.clear();
                chunk.other_entities.clear();
            }

            if (clear_static) {
                chunk.static_entities.clear();
            }
        }
    }
}

void ChunkController::indexEntity(Entity *entity) {
    pair<int, int> chunk = getChunk(entity);
    int chunk_x = chunk.first;
    int chunk_y = chunk.second;
    if (inMap(dimensions, chunk_x, chunk_y)) {
        ChunkContainer &chunk = chunks[chunk_x][chunk_y];

        if (entity->is_static) chunk.static_entities.push_back(entity);
        else                   chunk.entities.push_back(entity);
    }
}
