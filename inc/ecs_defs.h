#pragma once

#include <ecs.h>
#include <ww_math.h>
#include <input.h>

typedef struct RigidBody {
  fp::vector velocity;
} RigidBody;

typedef struct Transform {
  fp::vector position;
} Transform;

typedef struct PlayerInput {
  const struct input::player_input *input;
} Input;

typedef struct Sprite {
  unsigned int sprite_handle;
  float width, height;
  // const char *texture;
} Sprite;



struct PlayerInputSystem: public System {
  void RegisterPlayer(Entity);
  void Update();
};

struct PhysicsSystem: public System {
  void RegisterEntity(Entity);
  void Update();
};

struct GraphicsSystem: public System {
  GraphicsSystem();
  void RegisterPlayer(Entity);
  void Update();

  unsigned int pbuffer;
};
