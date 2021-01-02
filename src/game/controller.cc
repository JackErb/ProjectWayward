#include <controller.h>
#include <ecs_defs.h>
#include <fp.h>
#include <iostream>

GameController::GameController() : coordinator_() {
  coordinator_.RegisterComponent<Hitbox>();
  coordinator_.RegisterComponent<Transform>();
  coordinator_.RegisterComponent<Sprite>();
  coordinator_.RegisterComponent<PlayerInput>();

  physics_system_ = coordinator_.RegisterSystem<PhysicsSystem>();
  graphics_system_ = coordinator_.RegisterSystem<GraphicsSystem>();
  player_input_system_ = coordinator_.RegisterSystem<PlayerInputSystem>();

  CreatePlayer(0,0);
  for (int i = -5; i <= 5; i++) {
    CreateTile(i * 500, -500);
  }
}

Entity GameController::CreatePlayer(int x, int y) {
  Entity player = coordinator_.CreateEntity();

  Hitbox hitbox;
  hitbox.type = Hitbox::entity_dynamic;
  hitbox.polygons[0] = fp::make_rectangle(0,0,750,850);
  coordinator_.AddComponent<Hitbox>(player, hitbox);

  Transform transform;
  transform.position = vector(fp::from_int(x), fp::from_int(y));
  coordinator_.AddComponent<Transform>(player, transform);

  Sprite sprite;
  sprite.width = 750;
  sprite.height = 850;
  coordinator_.AddComponent<Sprite>(player, sprite);

  coordinator_.AddComponent<PlayerInput>(player);

  player_input_system_->RegisterPlayer(player);
  graphics_system_->RegisterPlayer(player);
  physics_system_->RegisterEntity(player);
  return player;
}

Entity GameController::CreateTile(int x, int y) {
  Entity tile = coordinator_.CreateEntity();

  Hitbox hitbox;
  hitbox.type = Hitbox::entity_static;
  hitbox.polygons[0] = fp::make_rectangle(0,0,500,500);
  coordinator_.AddComponent<Hitbox>(tile, hitbox);

  Transform transform;
  transform.position = vector(fp::from_int(x), fp::from_int(y));
  transform.velocity = vector();
  coordinator_.AddComponent<Transform>(tile, transform);

  Sprite sprite;
  sprite.width = 500;
  sprite.height = 500;
  coordinator_.AddComponent<Sprite>(tile, sprite);

  graphics_system_->RegisterTile(tile);
  physics_system_->RegisterEntity(tile);
  return tile;
}

void GameController::Update() {
  player_input_system_->Update();
  physics_system_->Update();
  graphics_system_->Update();
}
