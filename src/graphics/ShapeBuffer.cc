#include "ShapeBuffer.h"
#include "WaywardGL.h"
#include <glad/glad.h>
#include "ResourceManager.h"
#include "TextureLoader.h"

#include <iostream>
#include <vector>
#include <string>

using std::vector;
using std::string;

void ShapeBuffer::init(int max) {
    MaxShapes = max;
    vertices = new float[MaxShapes * VerticesLen];

    // Generate buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind buffers
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, MaxShapes * VerticesLen * sizeof(float), NULL, GL_DYNAMIC_DRAW);

    // Create vertex attributes
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, VerticesLen * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, VerticesLen * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // Unbind buffer
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ShapeBuffer::setShader(unsigned int shader_handle) {
    ShaderProg = shader_handle;
}

unsigned int ShapeBuffer::addShape(float x, float y, float w, float h) {
    if (index >= MaxShapes) {
        std::cerr << "Ran out of shape space WaywardGL" << std::endl;
        exit(1);
    }
    int vert_index = index * VerticesLen;
    vertices[vert_index]   = x;
    vertices[vert_index+1] = y;
    vertices[vert_index+2] = w;
    vertices[vert_index+3] = h;

    index += 1;
    return index - 1;
}

void ShapeBuffer::setShapePos(unsigned int sprite_handle, float x, float y) {
    int vert_index = sprite_handle * VerticesLen;
    vertices[vert_index]   = x;
    vertices[vert_index+1] = y;
}

void ShapeBuffer::updateBuffer() {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    int vertices_size = index * VerticesLen * sizeof(float);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices_size, vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ShapeBuffer::render(WaywardGL::DisplayData d) {
    updateBuffer();

    glBindVertexArray(VAO);
    glUseProgram(ShaderProg);
    glUniform2f(glGetUniformLocation(ShaderProg, "screenSize"), d.WindowWidth, d.WindowHeight);
    glUniform2f(glGetUniformLocation(ShaderProg, "cameraPos"), d.CameraX, d.CameraY);
    glUniform1f(glGetUniformLocation(ShaderProg, "screenScale"), d.WindowScale);
    glDrawArrays(GL_POINTS, 0, index);

    glBindVertexArray(0);
}
