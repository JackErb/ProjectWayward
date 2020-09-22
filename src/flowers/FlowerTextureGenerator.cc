#include "FlowerGenerator.h"

#include <iostream>
#include <ShaderLoader.h>
#include <glad/glad.h>

using std::cerr;
using std::endl;


static unsigned int FlowerVAO;
static unsigned int FlowerVBO;

// Flower vertices
static const int MAX_VERTICES = 1000;
static float *FlowerVertices;
static int FlowerIndex;

static unsigned int StemShaderProg;

static bool Initialized = false;
void init() {
    if (Initialized) return;
    // FlowerVertices contains (x,y) of vertices to draw
    FlowerVertices = new float[MAX_VERTICES * 2];

    // Generate buffers
    glGenVertexArrays(1, &FlowerVAO);
    glGenBuffers(1, &FlowerVBO);

    // Bind buffers
    glBindVertexArray(FlowerVAO);
    glBindBuffer(GL_ARRAY_BUFFER, FlowerVBO);
    glBufferData(GL_ARRAY_BUFFER, MAX_VERTICES * 2 * sizeof(float), FlowerVertices, GL_STATIC_DRAW);

    // Create vertex attributes
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind buffer
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    StemShaderProg = loadShaderProgram("stem.vert", "stem.frag");

    Initialized = true;
}

void drawStem(const Flower &);
void drawBud(const Flower &);

unsigned int generateFlowerTexture(const Flower &flower) {
    if (!Initialized) init();

    // Generate framebuffer
    unsigned int fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0,0,flower.width,flower.height);

    // Generate texture
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, flower.width, flower.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Attach texture to framebuffer then draw the flower
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
    drawStem(flower);
    drawBud(flower);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        cerr << "Unable to bind framebuffer" << endl;
        return 0;
    }

    glBindTexture(GL_TEXTURE_2D, texture);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &fbo);
    return texture;
}

void pushPoint(float x, float y) {
    if (FlowerIndex >= MAX_VERTICES) {
        std::cerr << "Ran out of space for vertices in flower texture generator." << std::endl;
        exit(1);
    }

    FlowerVertices[FlowerIndex*2] = x;
    FlowerVertices[FlowerIndex*2+1] = y;
    FlowerIndex += 1;
}

void drawSquare(const Flower &flower, const Point &p1, const Point &p2) {
    FlowerIndex = 0;
    pushPoint(-flower.width/2,-flower.height/2);
    pushPoint(-flower.width/2,flower.height/2);
    pushPoint(flower.width/2, -flower.height/2);
    pushPoint(flower.width/2, flower.height/2);

    glBindBuffer(GL_ARRAY_BUFFER, FlowerVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, FlowerIndex * 2 * sizeof(float), FlowerVertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(FlowerVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, FlowerIndex);
    glBindVertexArray(0);
}

void drawStem(const Flower &flower) {
    glUseProgram(StemShaderProg);
    glUniform2f(glGetUniformLocation(StemShaderProg, "size"), flower.width, flower.height);
    drawSquare(flower, flower.stem.points[0], flower.stem.points[1]);
}

void drawBud(const Flower &flower) {

}
