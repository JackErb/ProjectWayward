#include "MetaballBuffer.h"
#include <glad/glad.h>
#include <iostream>

static const float Vertices[] = {
    -1.0,  1.0, // top left
    -1.0, -1.0, // bottom left
     1.0,  1.0, // top right
     1.0, -1.0, // bottom right
};

void MetaballBuffer::init(int max, std::string id) {
    MaxBalls = max;
    this->id = id;

    MetaballData = new float[MaxBalls * DataLen];
    index = 0;

    // Generate buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind buffers
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), Vertices, GL_STATIC_DRAW);

    // Create vertex attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void MetaballBuffer::setShader(unsigned int handle) {
    ShaderProg = handle;
}

void MetaballBuffer::updateBuffer() {
    glUniform3fv(glGetUniformLocation(ShaderProg, "metaballs"), index, MetaballData);
    glUniform1i(glGetUniformLocation(ShaderProg, "metaballsSize"), index);
}

void MetaballBuffer::render(WaywardGL::DisplayData d) {
    glBindVertexArray(VAO);
    glUseProgram(ShaderProg);
    updateBuffer();

    glUniform2f(glGetUniformLocation(ShaderProg, "screenSize"), d.WindowWidth, d.WindowHeight);
    glUniform2f(glGetUniformLocation(ShaderProg, "cameraPos"), d.CameraX, d.CameraY);
    glUniform1f(glGetUniformLocation(ShaderProg, "screenScale"), d.WindowScale);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

unsigned int MetaballBuffer::addMetaball(float x, float y, float r) {
    if (index >= MaxBalls) {
        std::cerr << "Ran out of metaball space : " << id << std::endl;
        exit(1);
    }

    MetaballData[index*DataLen + 0] = x;
    MetaballData[index*DataLen + 1] = y;
    MetaballData[index*DataLen + 2] = r;

    return index++;
}

void MetaballBuffer::setMetaballPos(unsigned int handle, float x, float y) {
    MetaballData[handle*DataLen + 0] = x;
    MetaballData[handle*DataLen + 1] = y;
}

void MetaballBuffer::setMetaballRadius(unsigned int handle, float r) {
    MetaballData[handle*DataLen + 2] = r;
}
