#ifndef ChunkController_h
#define ChunkController_h

#include <ww_math.h>
#include <set>
#include <list>
#include <mutex>

class GameController;
class PhysicsController;
class Entity;
class StackAllocator;
class Chunk;

typedef struct MapDimensions {
    int map_width, map_height;
    int chunk_width, chunk_height;
    int tile_size;
} MapDimensions;

typedef struct ChunkContainer {
    std::mutex chunk_mutex;

    Vector2D position;
    std::list<Entity*> entities;
    std::list<Entity*> static_entities;
} ChunkContainer;

class ChunkController {
  public:
    ChunkController(GameController *gc, const MapDimensions &dimensions);
    ~ChunkController();

    void generateMap(StackAllocator &alloc);
    
    void updatePartitionForChunk(int x, int y);
    void setPhysicsController(PhysicsController *pc);

    ChunkContainer &getChunk(int x, int y) {
        return chunks[x][y];
    }

    int width() { return dimensions.map_width; }
    int height() { return dimensions.map_height; }

  private:
    void updatePartitionForEntity(Entity *entity);

    GameController *game_controller;
    MapDimensions dimensions;
    PhysicsController *physics_controller;
    
    Vector2D chunk_size;
    ChunkContainer **chunks;
};

#endif