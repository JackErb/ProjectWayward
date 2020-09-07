#ifndef ShapeBuffer_h
#define ShapeBuffer_h

#include "WaywardGL.h"
#include <vector>
#include <string>

typedef struct ShapeBuffer {
    unsigned int ShaderProg;
    unsigned int VAO;
    unsigned int VBO;
    int MaxShapes;

    int index = 0;
    float *vertices = nullptr;

    const int VerticesLen = 4;

    void init(int max);
    void setShader(unsigned int shader_handle);
    void updateBuffer();
    void render(WaywardGL::DisplayData d);

    unsigned int addShape(float x, float y, float w, float h);
    void setShapePos(unsigned int sprite_handle, float x, float y);
} ShapeBuffer;

#endif  /* ShapeRenderer_h */
