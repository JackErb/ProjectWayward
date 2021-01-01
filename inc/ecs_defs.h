#pragma once

#include <ww_math.h>
#include <input.h>

typedef struct RigidBody {
  fp::vector velocity;
} RigidBody;

typedef struct Transform {
  fp::vector position;
} Transform;

typedef struct Input {
  struct player_input *input;
} Input;

typedef struct Sprite {
  float width, height;
  const char *texture;
} Sprite;





struct PlayerInputSystem: public System {
  void Update() {}
};

struct PhysicsSystem: public System {
  void Update() {}
};

struct GraphicsSystem: public System {
  void Update();
};
