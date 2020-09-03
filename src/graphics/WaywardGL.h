#ifndef WaywardGL_h
#define WaywardGL_h

#include <string>
#include <unordered_map>

namespace WaywardGL {

void init(int width, int height);
void render();
void deinit();

int addSprite(float w, float h);
void updateSpritePos(int sprite_handle, float x, float y);

unsigned int loadTexture(std::string file_name);

void setTexture(unsigned int texture_handle);
}

#endif  /* WaywardGL_h */
