#pragma once

#include <ecs.h>
#include <ww_math.h>
#include <input.h>
#include <array>

static const int MaxPolygons = 1;
typedef struct Hitbox {
  enum {
    entity_dynamic, entity_static
  } type;
  std::array<fp::polygon, MaxPolygons> polygons;
} Hitbox;

typedef struct Transform {
  fp::vector position;
  fp::vector velocity;
  bool grounded;
} Transform;

typedef struct PlayerState {
  enum {
    grounded, airborne
  } state;
  int frame;
} PlayerState;

typedef struct PlayerInput {
  const struct input::player_input *input;
} Input;

typedef struct Sprite {
  unsigned int buffer_handle;
  unsigned int sprite_handle;
  float width, height;
  // const char *texture;
} Sprite;



struct PlayerInputSystem: public System {
  void RegisterPlayer(Entity);
  void Update();

 private:
  void UpdatePlayer(Entity);
};

struct PhysicsSystem: public System {
  void RegisterEntity(Entity);
  void Update();

 private:
  std::set<Entity> static_entities;
};

struct GraphicsSystem: public System {
  GraphicsSystem();
  void RegisterTile(Entity);
  void RegisterPlayer(Entity);
  void Update();

 private:
  void UpdateCamera();

  Entity main_player_;
  unsigned int pbuffer_, tbuffer_;
};
