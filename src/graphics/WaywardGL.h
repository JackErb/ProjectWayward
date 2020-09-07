#ifndef WaywardGL_h
#define WaywardGL_h

#include <string>
#include <unordered_map>

typedef struct SpriteBuffer SpriteBuffer;
typedef struct ShapeBuffer ShapeBuffer;

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

SpriteBuffer *spriteBuffer();
SpriteBuffer *tileBuffer();
ShapeBuffer  *shapeBuffer();
}

#endif  /* WaywardGL_h */
