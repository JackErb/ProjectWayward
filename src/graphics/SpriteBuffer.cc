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

void SpriteBuffer::init(int max, std::string id, unsigned int texture_handle, bool is_texture_array) {
    this->id = id;
    MaxSprites = max;
    vertices = new float[MaxSprites * VerticesLen];
    textures = new int[MaxSprites * TexturesLen];

    TextureHandle = texture_handle;
    TextureType = is_texture_array ? GL_TEXTURE_2D_ARRAY : GL_TEXTURE_2D;

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

void SpriteBuffer::setTextureHandle(unsigned int texture_handle) {
    TextureHandle = texture_handle;
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

    int handle = index - 1;
    handle_to_index[handle] = index - 1;
    return handle;
}

void SpriteBuffer::setSpritePos(unsigned int sprite_handle, float x, float y) {
    int vert_index = handle_to_index[sprite_handle] * VerticesLen;
    vertices[vert_index]   = x;
    vertices[vert_index+1] = y;
}

void SpriteBuffer::setSpriteSize(unsigned int sprite_handle, float x, float y) {
    int vert_index = handle_to_index[sprite_handle] * VerticesLen;
    vertices[vert_index+2]   = x;
    vertices[vert_index+3] = y;
}

void SpriteBuffer::setSpriteTexture(unsigned int sprite_handle, int t) {
    int text_index = handle_to_index[sprite_handle] * TexturesLen;
    textures[text_index] = t;
}

void SpriteBuffer::setSpriteDir(unsigned int sprite_handle, int dir) {
    int text_index = handle_to_index[sprite_handle] * TexturesLen;
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
    glBindTexture(TextureType, TextureHandle);

    glBindVertexArray(VAO);
    glUseProgram(ShaderProg);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glUniform2f(glGetUniformLocation(ShaderProg, "screenSize"), d.WindowWidth, d.WindowHeight);
    glUniform2f(glGetUniformLocation(ShaderProg, "cameraPos"), d.CameraX, d.CameraY);
    glUniform1f(glGetUniformLocation(ShaderProg, "screenScale"), d.WindowScale);
    glDrawArrays(GL_POINTS, 0, index);

    glBindVertexArray(0);
}

void SpriteBuffer::removeSprite(unsigned int sprite_handle) {
    // TODO: Update this to properly remove the sprite
    int sprite_index = handle_to_index[sprite_handle];

    float *vert_index_ptr = &vertices[sprite_index * VerticesLen];
    float *vert_end_ptr = &vertices[index * VerticesLen];
    for (int i = 0; i < VerticesLen; i++) {
        vert_index_ptr[i] = 0.f;//vert_end_ptr[i];
    }


    int *text_index_ptr = &textures[sprite_index * TexturesLen];
    int *text_end_ptr = &textures[index * TexturesLen];
    for (int i = 0; i < TexturesLen; i++) {
        text_index_ptr[i] = 0;//text_end_ptr[i];
    }
}
