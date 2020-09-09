#ifndef GameController_h
#define GameController_h

#include <vector>
#include "Input.h"
#include "PhysicsMultithreader.h"

class Entity;
struct StackAllocator;

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
};

#endif  /* GameController_h */
