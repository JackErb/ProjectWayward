#ifndef SpriteBuffer_h
#define SpriteBuffer_h

#include "WaywardGL.h"
#include <vector>
#include <string>

struct SpriteBuffer {
    unsigned int ShaderProg;
    unsigned int VAO;
    unsigned int VBO;
    int MaxSprites;
    unsigned int TextureArray;

    int index = 0;
    float *vertices = nullptr;

    void init(int max, std::vector<std::string> texture_files);
    void setShader(unsigned int shader_handle);
    void updateBuffer();
    void render(WaywardGL::DisplayData d);

    unsigned int addSprite(float x, float y, float w, float h);

    static const int VBOLen = 4;
};

#endif  /* SpriteRenderer_h */
