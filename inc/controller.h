#pragma once

#include <coordinator.h>
#include <ecs_defs.h>

class GameController {
 public:
  GameController();
  void Update();

 private:
   Entity player_;
   PhysicsSystem* physics_system_;
   GameStateCoordinator coordinator_;
};
