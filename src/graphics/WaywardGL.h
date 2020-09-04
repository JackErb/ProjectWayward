#ifndef WaywardGL_h
#define WaywardGL_h

#include <string>
#include <unordered_map>

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

unsigned int addSprite(float x, float y, float w, float h);
void updateSpritePos(unsigned int sprite_handle, float x, float y);
void updateSpriteTexture(unsigned int sprite_handle, unsigned int texture_handle);

unsigned int addShape(float x, float y, float w, float h);
void updateShapePos(unsigned int shape_handle, float x, float y);
}

#endif  /* WaywardGL_h */
