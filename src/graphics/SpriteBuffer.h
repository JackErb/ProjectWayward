#ifndef SpriteBuffer_h
#define SpriteBuffer_h

#include "WaywardGL.h"
#include <vector>
#include <string>
#include <unordered_map>

typedef struct SpriteBuffer {
    unsigned int ShaderProg;
    unsigned int VAO;
    unsigned int VBO;
    unsigned int TextureHandle;
    int TextureType;
    int MaxSprites;

    int index = 0;
    float *vertices = nullptr;
    int   *textures = nullptr;

    std::string id;

    const int VerticesLen = 4;
    const int TexturesLen = 2;

    void init(int max, std::string id, unsigned int texture_handle, bool is_texture_array);
    void setShader(unsigned int shader_handle);
    void setTextureHandle(unsigned int texture_handle);
    void updateBuffer();
    void render(WaywardGL::DisplayData d);

    unsigned int addSprite(float x, float y, float w, float h, int t);
    void setSpritePos(unsigned int sprite_handle, float x, float y);
    void setSpriteSize(unsigned int sprite_handle, float x, float y);
    void setSpriteTexture(unsigned int sprite_handle, int t);
    void setSpriteDir(unsigned int sprite_handle, int dir);
    void removeSprite(unsigned int sprite_handle);

    std::unordered_map<unsigned int, int> handle_to_index;
} SpriteBuffer;

#endif  /* SpriteRenderer_h */
