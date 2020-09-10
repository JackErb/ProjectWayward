#ifndef MetaballBuffer_h
#define MetaballBuffer_h

#include "WaywardGL.h"
#include <string>

typedef struct MetaballBuffer {
    int MaxBalls;
    unsigned int VAO;
    unsigned int VBO;
    unsigned int ShaderProg;

    void init(int max, std::string id);
    void setShader(unsigned int shader_handle);
    void updateBuffer();
    void render(WaywardGL::DisplayData d);

    const int DataLen = 3;
    float *MetaballData;
    int index = 0;

    std::string id;

    unsigned int addMetaball(float x, float y, float r);
    void setMetaballPos(unsigned int handle, float x, float y);
} MetaballBuffer;

#endif  /* MetaballBuffer_h */
