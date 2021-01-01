#include <controller.h>
#include <fp.h>
#include <iostream>

GameController::GameController() : coordinator_() {
  coordinator_.RegisterComponent<RigidBody>();
  coordinator_.RegisterComponent<Transform>();

  physics_system_ = coordinator_.RegisterSystem<PhysicsSystem>();
  player_ = coordinator_.CreateEntity();
}

void GameController::Update() {
  physics_system_->Update();
}
