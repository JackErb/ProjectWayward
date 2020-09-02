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

unsigned int PlayerTexture;

static const int SpriteVBOLen = 4;
static float SpriteVertices[400];
static int SpriteIndex = 0;
static unsigned int SpriteShaderProg;
static unsigned int SpriteVAO;
static unsigned int SpriteVBO;

void WaywardGL::init(int width, int height) {
	glEnable(GL_MULTISAMPLE);

	windowWidth = width;
	windowHeight = height;
	windowScale = 0.5;

	float vertices[] = {
	    // position  // size
		0.0f, 0.0f,  1500.0f, 1700.0f
	};

	SpriteShaderProg = loadShaderProgram();

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

	PlayerTexture = loadTexture("jump_3.png");

	glEnable(GL_BLEND);
}

void WaywardGL::render() {
    glBindBuffer(GL_ARRAY_BUFFER, SpriteVBO);
    glBufferData(GL_ARRAY_BUFFER, SpriteIndex * SpriteVBOLen * sizeof(float), SpriteVertices, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glActiveTexture(GL_TEXTURE0);
 	glBindTexture(GL_TEXTURE_2D, PlayerTexture);
	glUseProgram(SpriteShaderProg);
 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glUniform2f(glGetUniformLocation(SpriteShaderProg, "screenSize"), windowWidth, windowHeight);
	glUniform2f(glGetUniformLocation(SpriteShaderProg, "cameraPos"), 0.f, 0.f);
	glUniform1f(glGetUniformLocation(SpriteShaderProg, "screenScale"), windowScale);

	glBindVertexArray(SpriteVAO);
	glDrawArrays(GL_POINTS, 0, SpriteIndex);
}

void WaywardGL::deinit() {
	// Clean up resources
	glDeleteVertexArrays(1, &SpriteVAO);
}

int WaywardGL::addSprite(float w, float h) {
	int spriteIndex = SpriteIndex * SpriteVBOLen;
	SpriteVertices[spriteIndex + 0] = 0.f;
	SpriteVertices[spriteIndex + 1] = 0.f;
	SpriteVertices[spriteIndex + 2] = w;
	SpriteVertices[spriteIndex + 3] = h;
	return SpriteIndex++;
}

void WaywardGL::updateSpritePos(int sprite_handle, float x, float y) {
	int spriteIndex = sprite_handle * SpriteVBOLen;
	SpriteVertices[spriteIndex] = x;
	SpriteVertices[spriteIndex + 1] = y;
}
