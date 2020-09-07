#ifndef SpriteBuffer_h
#define SpriteBuffer_h

#include "WaywardGL.h"
#include <vector>
#include <string>

typedef struct SpriteBuffer {
    unsigned int ShaderProg;
    unsigned int VAO;
    unsigned int VBO;
    int MaxSprites;
    unsigned int TextureArray;

    int index = 0;
    float *vertices = nullptr;
    int   *textures = nullptr;

    const int VerticesLen = 4;
    const int TexturesLen = 2;

    void init(int max, std::vector<std::string> texture_files);
    void setShader(unsigned int shader_handle);
    void updateBuffer();
    void render(WaywardGL::DisplayData d);

    unsigned int addSprite(float x, float y, float w, float h, int t);
    void setSpritePos(unsigned int sprite_handle, float x, float y);
    void setSpriteTexture(unsigned int sprite_handle, int t);
    void setSpriteDir(unsigned int sprite_handle, int dir);
} SpriteBuffer;

#endif  /* SpriteRenderer_h */
