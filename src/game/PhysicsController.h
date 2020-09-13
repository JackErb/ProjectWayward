#ifndef PhysicsController_h
#define PhysicsController_h

#include "PhysicsTypes.h"

#include <map>
#include <vector>

class GameController;
class Entity;

class PhysicsController {
  public:
    PhysicsController(GameController *gc);
    ~PhysicsController();

    void runCollisionChecks();
    std::vector<CollisionManifold> runCollisionChecks(Entity *entity);

    void notifyPriorityManifolds();
    void notifyRemainingManifolds();

  private:
    std::map<Entity*, std::vector<CollisionManifold>> manifolds;
    GameController *gc;
};

#endif  /* PhysicsController_h */
