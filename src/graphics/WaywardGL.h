#ifndef WaywardGL_h
#define WaywardGL_h

#include <string>
#include <unordered_map>

typedef struct SpriteBuffer SpriteBuffer;
typedef struct ShapeBuffer ShapeBuffer;
typedef struct MetaballBuffer MetaballBuffer;

namespace WaywardGL {

struct DisplayData {
    float WindowWidth;
    float WindowHeight;
    float WindowScale;
    float CameraX;
    float CameraY;
};

void init(int width, int height);
void render();
void deinit();

void renderSprite(float x, float y, float w, float h, unsigned int texture_handle);

SpriteBuffer   *spriteBuffer();
SpriteBuffer   *tileBuffer();
ShapeBuffer    *shapeBuffer();
MetaballBuffer *waterBuffer();
}

#endif  /* WaywardGL_h */
