#include <controller.h>
#include <ecs_defs.h>
#include <fp.h>
#include <iostream>

GameController::GameController() : coordinator_() {
  coordinator_.RegisterComponent<RigidBody>();
  coordinator_.RegisterComponent<Transform>();
  coordinator_.RegisterComponent<Sprite>();
  coordinator_.RegisterComponent<PlayerInput>();

  physics_system_ = coordinator_.RegisterSystem<PhysicsSystem>();
  graphics_system_ = coordinator_.RegisterSystem<GraphicsSystem>();
  player_input_system_ = coordinator_.RegisterSystem<PlayerInputSystem>();

  player_ = coordinator_.CreateEntity();
  coordinator_.AddComponent<RigidBody>(player_);
  coordinator_.AddComponent<Transform>(player_);
  coordinator_.AddComponent<Sprite>(player_);
  coordinator_.AddComponent<PlayerInput>(player_);

  player_input_system_->RegisterPlayer(player_);
  graphics_system_->RegisterPlayer(player_);
  physics_system_->RegisterEntity(player_);
}

void GameController::Update() {
  player_input_system_->Update();
  physics_system_->Update();
  graphics_system_->Update();
}
