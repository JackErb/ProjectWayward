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
            if (x == map_w / 2 && y == map_h / 2) continue;

            ChunkContainer &chunk_container =  chunks[x][y];
            int chunk_x = (x - map_w/2) * chunk_w * tile_size;
            int chunk_y = (y - map_h/2) * chunk_h * tile_size;
            
            chunk_container.position = { FixedPoint::fromInt(chunk_x), FixedPoint::fromInt(chunk_y) };
            for (int tx = 0; tx < chunk_w; tx++) {
                for (int ty = 0; ty < chunk_h; ty++) {
                    int chunkx = chunk_x + tx * tile_size;
                    int chunky = chunk_y + ty * tile_size;

                    void *ptr = alloc.raw_allocate<Chunk>();
                    Chunk *chunk = new (ptr) Chunk(chunkx, chunky, tile_size, tile_size);
                    game_controller->addEntity(chunk);
                }
            }
        }
    }

}

ChunkController::~ChunkController() {

}

void ChunkController::setPhysicsController(PhysicsController *pc) {
    physics_controller = pc;
}

bool inMap(const MapDimensions &dim, int x, int y) {
    return x >= 0 && x < dim.map_width && y >= 0 && y < dim.map_height;
}

void ChunkController::updatePartitionForChunk(int x, int y) {
    if (!inMap(dimensions, x, y)) return;

    ChunkContainer &chunk = chunks[x][y];
    auto it = chunk.entities.begin();
    while (it != chunk.entities.end()) {
        Entity *entity = *it;

        Vector2D position = (entity->data.position - chunks[0][0].position) / chunk_size;
        int new_x = position.x.toInt(), new_y = position.y.toInt();

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
   }
}
