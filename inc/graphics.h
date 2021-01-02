#pragma once

#include <vector>
#include <string>

namespace graphics {
typedef struct sprite_data {
  float x, y, w, h;  // size & position
  int text, dir;     // texture handle & direction
} SpriteData;

struct window_display {
  float width, height;
  float camera_x, camera_y;
  float scale;
};

/* GRAPHICS */
void Init();
void Render();

struct window_display GetDisplay();
void SetDisplay(const struct window_display &display);

/* BUFFERS */
unsigned int InitBuffer(int max_sprites);
void SetBufferShaderProg(int buf_handle, int shader_prog);
void SetBufferTexture(int buf_handle, int texture);

unsigned int AddSprite(int buf_handle);
void UpdateSprite(int buf_handle, int sprite_handle, const struct sprite_data &data);
void RemoveSprite(int buf_handle, int sprite_handle);

/* SHADERS */
unsigned int LoadShaderProgram(const std::string &vert, const std::string &geom,
                               const std::string &frag);

/* TEXTURES */
unsigned int LoadTexture(const std::string &file);
};
