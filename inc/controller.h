#pragma once

#include <coordinator.h>
#include <ecs_defs.h>

class GameController {
 public:
  GameController();
  void Update();

 private:
   Entity CreatePlayer(int x, int y);
   Entity CreateTile(int x, int y);

   Entity player_;
   PlayerInputSystem* player_input_system_;
   GraphicsSystem* graphics_system_;
   PhysicsSystem* physics_system_;
   GameStateCoordinator coordinator_;
};
