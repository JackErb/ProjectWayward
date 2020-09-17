#ifndef PhysicsController_h
#define PhysicsController_h

#include "PhysicsTypes.h"

#include <map>
#include <vector>

class GameController;
class ChunkController;
struct ChunkContainer;
class Entity;

class PhysicsController {
  public:
    PhysicsController(GameController *gc);
    ~PhysicsController();

    void setChunkController(ChunkController *cc);

    void runCollisionChecks();
    std::vector<CollisionManifold> runCollisionChecks(const ChunkContainer &chunk);

  private:
    void notifyPriorityManifolds(const std::map<Entity*, std::vector<CollisionManifold>> &manifolds);
    void notifyRemainingManifolds(const std::map<Entity*, std::vector<CollisionManifold>> &manifolds);

  private:
    std::map<Entity*, std::vector<CollisionManifold>> manifolds;
    GameController *game_controller;
    ChunkController *chunk_controller;
};

#endif  /* PhysicsController_h */
