#include "WaywardGL.h"

#include "resource_path.h"
#include "TextureLoader.h"
#include "Shader.h"
#include "SpriteBuffer.h"

#include <glad/glad.h>
#include <string>
#include <iostream>

using namespace WaywardGL;

using std::string;
using std::cout;
using std::cerr;
using std::endl;

static DisplayData Display;

static const int MaxSprites = 100;
static SpriteBuffer PlayerSpriteBuffer;

static const int MaxShapes = 5000;
static unsigned int ShapeVAO;

static const int ShapeVBOLen = 4;
static float ShapeVertices[ShapeVBOLen * MaxShapes];
static int ShapeIndex = 0;
static unsigned int ShapeVBO;
static unsigned int ShapeShaderProg;

void GLAPIENTRY
MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{
  cout << source << endl;
}


void WaywardGL::init(int width, int height) {
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);

    Display.WindowWidth = width;
    Display.WindowHeight = height;
    Display.WindowScale = 0.25;

    unsigned int shader_handle = loadShaderProgram("basic.vert", "basic.geom", "basic.frag");

    PlayerSpriteBuffer.init(MaxSprites, {"jump_3.png", "jump_2.png"});
    PlayerSpriteBuffer.setShader(shader_handle);


    // Shape
    ShapeShaderProg = loadShaderProgram("basic.vert", "basic.geom", "shape.frag");
    
    glGenVertexArrays(1, &ShapeVAO);
    glGenBuffers(1, &ShapeVBO);
    glBindVertexArray(ShapeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, ShapeVBO);
    glBufferData(GL_ARRAY_BUFFER, 0, ShapeVertices, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void WaywardGL::render() {
    Display.CameraX = -PlayerSpriteBuffer.vertices[0];
    Display.CameraY = -PlayerSpriteBuffer.vertices[1];

    PlayerSpriteBuffer.updateBuffer();
    PlayerSpriteBuffer.render(Display);

    glBindVertexArray(ShapeVAO);
    glUseProgram(ShapeShaderProg);
    glUniform2f(glGetUniformLocation(ShapeShaderProg, "screenSize"), Display.WindowWidth, Display.WindowHeight);
    glUniform2f(glGetUniformLocation(ShapeShaderProg, "cameraPos"), Display.CameraX, Display.CameraY);
    glUniform1f(glGetUniformLocation(ShapeShaderProg, "screenScale"), Display.WindowScale);
    glDrawArrays(GL_POINTS, 0, ShapeIndex);
}

void WaywardGL::deinit() {
    // Clean up resources
}

SpriteBuffer *WaywardGL::spriteBuffer() { return &PlayerSpriteBuffer; }

unsigned int WaywardGL::addShape(float x, float y, float w, float h) {
    if (ShapeIndex >= MaxShapes) {
        std::cerr << "Ran out of shape space WaywardGL" << std::endl;
        exit(1);
    }

    int shape_index = ShapeIndex * ShapeVBOLen;
    ShapeVertices[shape_index + 0] = x;
    ShapeVertices[shape_index + 1] = y;
    ShapeVertices[shape_index + 2] = w;
    ShapeVertices[shape_index + 3] = h;
    ShapeIndex += 1;

    glBindBuffer(GL_ARRAY_BUFFER, ShapeVBO);
    glBufferData(GL_ARRAY_BUFFER, ShapeIndex * ShapeVBOLen * sizeof(float), ShapeVertices, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return ShapeIndex - 1;
}

void WaywardGL::updateShapePos(unsigned int shape_handle, float x, float y) {
    int shapeIndex = shape_handle * ShapeVBOLen;
    ShapeVertices[shapeIndex] = x;
    ShapeVertices[shapeIndex + 1] = y;
    
    glBindBuffer(GL_ARRAY_BUFFER, ShapeVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, ShapeIndex * ShapeVBOLen * sizeof(float), ShapeVertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
