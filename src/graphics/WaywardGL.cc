#include "WaywardGL.h"

#include "resource_path.h"
#include "TextureLoader.h"
#include "Shader.h"

#include <glad/glad.h>
#include <string>
#include <iostream>

using namespace WaywardGL;

using std::string;
using std::cout;
using std::cerr;
using std::endl;

static float windowWidth;
static float windowHeight;
static float windowScale;

static const int SpriteVBOLen = 4;
static const int ShapeVBOLen = 4;
static const int MaxSprites = 100;

static unsigned int SpriteVAO;

static float SpriteVertices[SpriteVBOLen * MaxSprites];
static int SpriteIndex = 0;
static unsigned int SpriteVBO;
static unsigned int SpriteShaderProg;

static float ShapeVertices[ShapeVBOLen * MaxSprites];
static int ShapeIndex = 0;
static unsigned int ShapeVBO;
static unsigned int ShapeShaderProg;

void WaywardGL::init(int width, int height) {
	glEnable(GL_MULTISAMPLE);

	windowWidth = width;
	windowHeight = height;
	windowScale = 0.5;

	SpriteShaderProg = loadShaderProgram("basic.vert", "basic.geom", "basic.frag");
	ShapeShaderProg = loadShaderProgram("basic.vert", "shape.geom", "shape.frag");

    glGenVertexArrays(1, &SpriteVAO);
    glGenBuffers(1, &SpriteVBO);
	glBindVertexArray(SpriteVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, SpriteVBO);
    glBufferData(GL_ARRAY_BUFFER, 0, SpriteVertices, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
	// Unbind VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

	glEnable(GL_BLEND);
}

void WaywardGL::render() {
	glBindVertexArray(SpriteVAO);

    glBindBuffer(GL_ARRAY_BUFFER, SpriteVBO);
    glBufferData(GL_ARRAY_BUFFER, SpriteIndex * SpriteVBOLen * sizeof(float), SpriteVertices, GL_DYNAMIC_DRAW);

	glActiveTexture(GL_TEXTURE0);
	glUseProgram(SpriteShaderProg);
 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glUniform2f(glGetUniformLocation(SpriteShaderProg, "screenSize"), windowWidth, windowHeight);
	glUniform2f(glGetUniformLocation(SpriteShaderProg, "cameraPos"), 0.f, 0.f);
	glUniform1f(glGetUniformLocation(SpriteShaderProg, "screenScale"), windowScale);

	glDrawArrays(GL_POINTS, 0, SpriteIndex);



	glUseProgram(ShapeShaderProg);
	//glBindBuffer(GL_ARRAY_BUFFER, ShapeVBO);
	glBufferData(GL_ARRAY_BUFFER, ShapeIndex * ShapeVBOLen * sizeof(float), ShapeVertices, GL_DYNAMIC_DRAW);
	glUniform2f(glGetUniformLocation(ShapeShaderProg, "screenSize"), windowWidth, windowHeight);
	glUniform2f(glGetUniformLocation(ShapeShaderProg, "cameraPos"), 0.f, 0.f);
	glUniform1f(glGetUniformLocation(ShapeShaderProg, "screenScale"), windowScale);

	glDrawArrays(GL_POINTS, 0, ShapeIndex);
}

void WaywardGL::deinit() {
	// Clean up resources
	glDeleteVertexArrays(1, &SpriteVAO);
}

unsigned int WaywardGL::addSprite(float w, float h) {
	int spriteIndex = SpriteIndex * SpriteVBOLen;
	SpriteVertices[spriteIndex + 0] = 0.f;
	SpriteVertices[spriteIndex + 1] = 0.f;
	SpriteVertices[spriteIndex + 2] = w;
	SpriteVertices[spriteIndex + 3] = h;
	return SpriteIndex++;
}

void WaywardGL::updateSpritePos(unsigned int sprite_handle, float x, float y) {
	int spriteIndex = sprite_handle * SpriteVBOLen;
	SpriteVertices[spriteIndex] = x;
	SpriteVertices[spriteIndex + 1] = y;
}

unsigned int WaywardGL::addShape(float w, float h) {
	int shapeIndex = ShapeIndex * ShapeVBOLen;
	ShapeVertices[shapeIndex + 0] = 0.f;
	ShapeVertices[shapeIndex + 1] = 0.f;
	ShapeVertices[shapeIndex + 2] = w;
	ShapeVertices[shapeIndex + 3] = h;
	return ShapeIndex++;
}

void WaywardGL::updateShapePos(unsigned int shape_handle, float x, float y) {
	int shapeIndex = shape_handle * ShapeVBOLen;
	ShapeVertices[shapeIndex] = x;
	ShapeVertices[shapeIndex + 1] = y;
}

void WaywardGL::setTexture(unsigned int texture_handle) {
 	glBindTexture(GL_TEXTURE_2D, texture_handle);
}
