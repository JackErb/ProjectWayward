#include "SpriteBuffer.h"
#include "WaywardGL.h"
#include <glad/glad.h>
#include "ResourceManager.h"
#include "TextureLoader.h"

#include <iostream>
#include <vector>
#include <string>

using std::vector;
using std::string;

void SpriteBuffer::init(int max, vector<string> texture_files) {
    MaxSprites = max;
    vertices = new float[max * VBOLen];
 
    TextureArray = WaywardGL::loadTextures(texture_files);

    // Generate buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind buffers
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 0, vertices, GL_DYNAMIC_DRAW);

    // Create vertex attributes
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind buffer
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SpriteBuffer::setShader(unsigned int shader_handle) {
    ShaderProg = shader_handle;
}

unsigned int SpriteBuffer::addSprite(float x, float y, float w, float h) {
    if (index >= MaxSprites) {
        std::cerr << "Ran out of sprite space WaywardGL" << std::endl;
        exit(1);
    }
    int sprite_index = index * VBOLen;
    vertices[sprite_index]   = x;
    vertices[sprite_index+1] = y;
    vertices[sprite_index+2] = w;
    vertices[sprite_index+3] = h;

    index += 1;

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, index * VBOLen * sizeof(float), vertices, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return index - 1;
}

void SpriteBuffer::updateBuffer() {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, index * VBOLen * sizeof(float), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SpriteBuffer::render(WaywardGL::DisplayData d) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, TextureArray);

    glBindVertexArray(VAO);
    glUseProgram(ShaderProg);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glUniform2f(glGetUniformLocation(ShaderProg, "screenSize"), d.WindowWidth, d.WindowHeight);
    glUniform2f(glGetUniformLocation(ShaderProg, "cameraPos"), d.CameraX, d.CameraY);
    glUniform1f(glGetUniformLocation(ShaderProg, "screenScale"), d.WindowScale);
    glUniform1i(glGetUniformLocation(ShaderProg, "textures"), 0);
    glDrawArrays(GL_POINTS, 0, index);
}




