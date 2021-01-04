#include <ecs_defs.h>
#include <ecs.h>
#include <coordinator.h>
#include <logs.h>
#include <graphics.h>

// Graphics & Rendering
GraphicsSystem::GraphicsSystem() {
  pbuffer_ = graphics::InitBuffer(5);
  unsigned int pshader_prog = graphics::LoadShaderProgram("basic.vert", "basic.geom", "basic.frag");
  graphics::SetBufferShaderProg(pbuffer_, pshader_prog);
  graphics::SetBufferTexture(pbuffer_, graphics::LoadTexture("jump_3.png"));

  tbuffer_ = graphics::InitBuffer(1000);
  unsigned int tshader_prog = graphics::LoadShaderProgram("basic_tile.vert", "basic.geom", "basic.frag");
  graphics::SetBufferShaderProg(tbuffer_, tshader_prog);
  graphics::SetBufferTexture(tbuffer_, graphics::LoadTexture("tile_grass.png"));
}

void GraphicsSystem::RegisterPlayer(Entity player) {
  main_player_ = player;
  entities.insert(player);

  // Init sprite component
  Sprite *sprite_component = coordinator->GetComponent<Sprite>(player);
  sprite_component->buffer_handle = pbuffer_;
  sprite_component->sprite_handle = graphics::AddSprite(pbuffer_);
}

void GraphicsSystem::RegisterTile(Entity tile) {
  entities.insert(tile);

  // Init sprite component
  Sprite *sprite_component = coordinator->GetComponent<Sprite>(tile);
  sprite_component->buffer_handle = tbuffer_;
  sprite_component->sprite_handle = graphics::AddSprite(tbuffer_);
}

void GraphicsSystem::UpdateCamera() {
  Transform *transform = coordinator->GetComponent<Transform>(main_player_);

  struct graphics::window_display display = graphics::GetDisplay();
  display.camera_x = -fp::to_float(transform->position.x);
  display.camera_y = -fp::to_float(transform->position.y);
  graphics::SetDisplay(display);
}

void GraphicsSystem::Update() {
  UpdateCamera();

  for (Entity e : entities) {
    Transform *transform_component = coordinator->GetComponent<Transform>(e);
    Sprite *sprite_component = coordinator->GetComponent<Sprite>(e);

    float x = fp::to_float(transform_component->position.x);
    float y = fp::to_float(transform_component->position.y);
    float w = sprite_component->width;
    float h = sprite_component->height;
    graphics::SpriteData sprite_data = { x, y, w, h, 0, 1 };

    unsigned int buffer_handle = sprite_component->buffer_handle;
    unsigned int sprite_handle = sprite_component->sprite_handle;
    graphics::UpdateSprite(buffer_handle, sprite_handle, sprite_data);
  }
}
