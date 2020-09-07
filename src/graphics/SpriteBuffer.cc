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

void SpriteBuffer::init(int max, std::string id, vector<string> texture_files) {
    this->id = id;
    MaxSprites = max;
    vertices = new float[MaxSprites * VerticesLen];
    textures = new int[MaxSprites * TexturesLen];

    TextureArray = WaywardGL::loadTextures(texture_files);

    // Generate buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind buffers
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, MaxSprites * (VerticesLen * sizeof(float) + TexturesLen * sizeof(int)), NULL, GL_DYNAMIC_DRAW);

    // Create vertex attributes
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, VerticesLen * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, VerticesLen * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    long pointer = MaxSprites * VerticesLen * sizeof(float);
    glVertexAttribIPointer(2, 1, GL_INT, TexturesLen * sizeof(int), (void*)pointer);
    glEnableVertexAttribArray(2);
    glVertexAttribIPointer(3, 1, GL_INT, TexturesLen * sizeof(int), (void*)(pointer + 1 * sizeof(int)));
    glEnableVertexAttribArray(3);

    // Unbind buffer
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SpriteBuffer::setShader(unsigned int shader_handle) {
    ShaderProg = shader_handle;
}

unsigned int SpriteBuffer::addSprite(float x, float y, float w, float h, int t) {
    if (index >= MaxSprites) {
        std::cerr << "Ran out of sprite space : " << id << std::endl;
        exit(1);
    }
    int vert_index = index * VerticesLen;
    vertices[vert_index]   = x;
    vertices[vert_index+1] = y;
    vertices[vert_index+2] = w;
    vertices[vert_index+3] = h;

    int text_index = index * TexturesLen;
    textures[text_index] = t;
    textures[text_index+1] = 1;

    index += 1;
    return index - 1;
}

void SpriteBuffer::setSpritePos(unsigned int sprite_handle, float x, float y) {
    int vert_index = sprite_handle * VerticesLen;
    vertices[vert_index]   = x;
    vertices[vert_index+1] = y;
}

void SpriteBuffer::setSpriteTexture(unsigned int sprite_handle, int t) {
    int text_index = sprite_handle * TexturesLen;
    textures[text_index] = t;
}

void SpriteBuffer::setSpriteDir(unsigned int sprite_handle, int dir) {
    int text_index = sprite_handle * TexturesLen;
    textures[text_index+1] = dir;
}

void SpriteBuffer::updateBuffer() {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    int vertices_size = index * VerticesLen * sizeof(float);
    int textures_size = index * TexturesLen * sizeof(int);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices_size, vertices);
    glBufferSubData(GL_ARRAY_BUFFER, MaxSprites * VerticesLen * sizeof(float), textures_size, textures);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SpriteBuffer::render(WaywardGL::DisplayData d) {
    updateBuffer();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, TextureArray);

    glBindVertexArray(VAO);
    glUseProgram(ShaderProg);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glUniform2f(glGetUniformLocation(ShaderProg, "screenSize"), d.WindowWidth, d.WindowHeight);
    glUniform2f(glGetUniformLocation(ShaderProg, "cameraPos"), d.CameraX, d.CameraY);
    glUniform1f(glGetUniformLocation(ShaderProg, "screenScale"), d.WindowScale);
    glDrawArrays(GL_POINTS, 0, index);

    glBindVertexArray(0);
}
