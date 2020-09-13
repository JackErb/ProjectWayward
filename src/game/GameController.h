#ifndef GameController_h
#define GameController_h

#include <vector>
#include "Input.h"
#include "PhysicsController.h"

class Entity;
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
    void removeEntity(Entity *entity);

    PhysicsController physics;
    std::vector<Entity*> entities;
    std::vector<WaterEntity*> water_entities;

    PlayerInput player_input;
};

#endif  /* GameController_h */
