#ifndef GameController_h
#define GameController_h

#include <vector>
#include "Input.h"
#include "PhysicsMultithreader.h"
#include "fluids/WaterBall.h"

class Entity;
class StackAllocator;

class GameController {
  public:
    GameController();
    ~GameController();

    void pretick();
    void tick();
    void render();

    StackAllocator *allocator();
    void addEntity(Entity *entity);
    void removeEntity(Entity *entity);

    std::vector<PhysicsMultithreader::JobReturn> runCollisionChecks(Entity *entity);

  private:
    PlayerInput player_input;
    std::vector<Entity*> entities;
 
    static constexpr int num_water = 11;
    WaterBall water[num_water];
};

#endif  /* GameController_h */
