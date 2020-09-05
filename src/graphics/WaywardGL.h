#ifndef WaywardGL_h
#define WaywardGL_h

#include <string>
#include <unordered_map>

struct SpriteBuffer;

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

unsigned int addShape(float x, float y, float w, float h);
void updateShapePos(unsigned int shape_handle, float x, float y);

SpriteBuffer *spriteBuffer();
}

#endif  /* WaywardGL_h */
