#include <ecs_defs.h>
#include <ecs.h>
#include <coordinator.h>
#include <logs.h>

#include <ww_math.h>
#include <fp_overloads.h>
#include <input.h>
#include <graphics.h>
#include <graphics.h>
#include <iostream>

// Physics
void PhysicsSystem::RegisterEntity(Entity e) {
  entities.insert(e);
}

void PhysicsSystem::Update() {
  for (Entity e: entities) {

  }
}

// PlayerInput
void PlayerInputSystem::RegisterPlayer(Entity player) {
  entities.insert(player);

  // Init input component
  Input *input = coordinator->GetComponent<Input>(player);
  input->input = input::GetInput(0);
}

void PlayerInputSystem::Update() {
  for (Entity e : entities) {
    Input *input_component = coordinator->GetComponent<Input>(e);
    Transform *transform = coordinator->GetComponent<Transform>(e);

    const struct input::player_input *input = input_component->input;
    transform->position.x += fp::from_int(input->stick_x * 100);
    transform->position.y += fp::from_int(input->stick_y * 100);
    std::cout << input->stick_x * 75 << " " << fp::to_int(transform->position.x) << std::endl;
  }
}

// Graphics & Rendering
GraphicsSystem::GraphicsSystem() {
  pbuffer = graphics::InitBuffer(10);
  graphics::SetBufferShaderProg(pbuffer, graphics::LoadShaderProgram("basic.vert", "basic.geom", "basic.frag"));
  graphics::SetBufferTexture(pbuffer, graphics::LoadTexture("jump_3.png"));
}

void GraphicsSystem::RegisterPlayer(Entity player) {
  entities.insert(player);

  // Init sprite component
  Sprite *sprite_component = coordinator->GetComponent<Sprite>(player);
  sprite_component->width = 750;
  sprite_component->height = 850;
  sprite_component->sprite_handle = graphics::AddSprite(pbuffer);
}

void GraphicsSystem::Update() {
  for (Entity e : entities) {
    Transform *transform_component = coordinator->GetComponent<Transform>(e);
    Sprite *sprite_component = coordinator->GetComponent<Sprite>(e);

    float x = fp::to_float(transform_component->position.x);
    float y = fp::to_float(transform_component->position.y);
    std::cout << x << std::endl;
    float w = sprite_component->width;
    float h = sprite_component->height;
    graphics::SpriteData sprite_data = {x, y, w, h, 0, 1};

    unsigned int sprite_handle = sprite_component->sprite_handle;
    graphics::UpdateSprite(pbuffer, sprite_handle, sprite_data);
  }
}
