#ifndef GameController_h
#define GameController_h

#include <ww_math.h>
#include <vector>
#include "Input.h"
#include "PhysicsController.h"
#include "ChunkController.h"

class Entity;
class Chunk;
class WaterEntity;
class StackAllocator;

class GameController {
  public:
    GameController();
    ~GameController();

    void pretick();
    void tick();
    void render();

    StackAllocator *allocator();

    void addWaterEntity(WaterEntity *entity);
    void addEntity(Entity *entity);
    void addStaticEntity(Entity *entity);
    void removeEntity(Entity *entity);

  private:
    PhysicsController physics;
    ChunkController chunks;

    unsigned int entity_id = 0;
    std::vector<Entity*> entities;
    std::vector<Entity*> static_entities;
    std::vector<WaterEntity*> water_entities;

    bool chunk_reindex = false;

    PlayerInput player_input;
};

#endif  /* GameController_h */
