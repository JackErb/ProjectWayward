#include <graphics.h>
#include <logs.h>

#include <glad.h>
#include <iostream>

using std::vector;
using std::string;

/* ***************************** */
/*         STATIC DATA           */
/* ***************************** */

#define MAX_BUFFERS 10

// A sprite consists of four floats (x, y, width, height)
// and two ints (texture index, direction)
using graphics::sprite_data;

struct buffer {
  bool in_use;
  bool updated;

  unsigned int vao, vbo;
  unsigned int shader_prog, texture;
  int max_sprites;
  int index;
  struct sprite_data *sprites;
};

int buffers_index;
struct buffer buffers[MAX_BUFFERS];
struct graphics::window_display display;


/* GRAPHICS */
void graphics::Init() {
  buffers_index = 0;
  for (int i = 0; i < MAX_BUFFERS; i++) {
    buffers[i].in_use = false;
    buffers[i].updated = true;
  }

  glEnable(GL_BLEND);
  glEnable(GL_MULTISAMPLE);
}

void graphics::Render() {
  for (int i = 0; i < MAX_BUFFERS; i++) {
    struct buffer *buf = &buffers[i];
    if (!buf->in_use || buf->index == 0) continue;

    if (buf->updated) {
      // Send updated sprite data to GPU
      glBindBuffer(GL_ARRAY_BUFFER, buf->vbo);
      glBufferSubData(GL_ARRAY_BUFFER, 0, buf->index * sizeof(struct sprite_data), buf->sprites);
      glBindBuffer(GL_ARRAY_BUFFER, 0);

      buf->updated = false;
    }

    // Rendering setup
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, buf->texture);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set shader program uniforms
    int prog = buf->shader_prog;
    glUseProgram(buf->shader_prog);
    glUniform2f(glGetUniformLocation(prog, "screenSize"), display.width, display.height);
    glUniform2f(glGetUniformLocation(prog, "cameraPos"), display.camera_x, display.camera_y);
    glUniform1f(glGetUniformLocation(prog, "screenScale"), display.scale);

    // Bind the buffer's VAO & shader program then draw to screen
    glBindVertexArray(buf->vao);
    glDrawArrays(GL_POINTS, 0, buf->index);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
    glBindVertexArray(0);
  }
}


/* BUFFERS */
unsigned int graphics::InitBuffer(int max_sprites) {
  int i;
  for (i = 0; i < MAX_BUFFERS; i++) {
    buffers_index = (buffers_index + 1) % MAX_BUFFERS;
    if (!buffers[buffers_index].in_use) break;
  }

  if (i == MAX_BUFFERS) {
    fatalerror("Buffers array is full. Cannot initialize another.");  
  }

  struct buffer *buffer = &buffers[buffers_index];

  // Generate buffers
  glGenVertexArrays(1, &buffer->vao);
  glGenBuffers(1, &buffer->vbo);

  // Generate VBO
  glBindVertexArray(buffer->vao);
  glBindBuffer(GL_ARRAY_BUFFER, buffer->vbo);
  glBufferData(GL_ARRAY_BUFFER, max_sprites * sizeof(struct sprite_data), NULL, GL_DYNAMIC_DRAW);

  // Build vertex attributes
  int stride = sizeof(struct sprite_data);
  // Position vector
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
  glEnableVertexAttribArray(0);
  // Size vector
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // Texture handle
  glVertexAttribIPointer(2, 1, GL_INT, stride, (void*)(4 * sizeof(float)));
  glEnableVertexAttribArray(2);
  // Direction
  glVertexAttribIPointer(3, 1, GL_INT, stride, (void*)(4 * sizeof(float) + sizeof(int)));
  glEnableVertexAttribArray(3);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // @Todo: Make sure this is deallocated.
  buffer->sprites = new sprite_data[max_sprites];
  buffer->max_sprites = max_sprites;
  buffer->index = 0;
  buffer->in_use = true;

  int ret_handle = buffers_index;
  buffers_index = (buffers_index + 1) % MAX_BUFFERS;
  return ret_handle;
}

void graphics::SetBufferShaderProg(int buf_handle, int shader) {
  struct buffer *buf = &buffers[buf_handle];
  fassert(buf->in_use, "SetBufferShaderProg() : invalid buf handle");
  buf->shader_prog = shader;
}

void graphics::SetBufferTexture(int buf_handle, int texture) {
  struct buffer *buf = &buffers[buf_handle];
  fassert(buf->in_use, "SetBufferTexture() : invalid buf handle");
  buf->texture = texture;
}

unsigned int graphics::AddSprite(int buf_handle) {
  fassert(buf_handle < MAX_BUFFERS, "AddSprite() : invalid buf handle");

  struct buffer *buf = &buffers[buf_handle];
  fassert(buf->in_use, "AddSprite() : buf not in use");
  fassert(buf->index < buf->max_sprites, "AddSprite() : no more space for sprites");

  int handle = buf->index;
  buf->index++;
  return handle;
}

void graphics::UpdateSprite(int buf_handle, int sprite_handle, const struct sprite_data &data) {
  fassert(buf_handle < MAX_BUFFERS, "UpdateSprite() : invalid buf handle");

  struct buffer *buf = &buffers[buf_handle];
  fassert(buf->in_use, "UpdateSprite() : buf not in use");
  fassert(sprite_handle < buf->max_sprites, "UpdateSprite() : invalid sprite handle");

  buf->sprites[sprite_handle] = data;
  buf->updated = true;
}

void graphics::RemoveSprite(int buf_handle, int sprite_handle) {
  fatalerror("RemoveSprite() : not implemented.");
}

void graphics::SetDisplay(const struct window_display &win_display) {
  display = win_display;
}
