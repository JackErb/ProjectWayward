#include "WaywardGL.h"

#include "resource_path.h"
#include "TextureLoader.h"
#include "Shader.h"
#include "SpriteBuffer.h"
#include "ShapeBuffer.h"

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

static const int MaxTiles = 3000;
static SpriteBuffer TileSpriteBuffer;

static const int MaxShapes = 5000;
static ShapeBuffer ShapeDisplayBuffer;

void WaywardGL::init(int width, int height) {
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);

    Display.WindowWidth = width;
    Display.WindowHeight = height;
    Display.WindowScale = 0.2;

    unsigned int shader_handle = loadShaderProgram("basic.vert", "basic.geom", "basic.frag");

    PlayerSpriteBuffer.init(MaxSprites, {"jump_0.png", "jump_1.png", "jump_2.png", "jump_3.png",
                                         "land_0.png", "land_1.png", "land_2.png", "land_3.png"});
    PlayerSpriteBuffer.setShader(shader_handle);

    TileSpriteBuffer.init(MaxTiles, {"tile_grass.png", "tile_grass2.png",
                                     "tile_sand.png", "tile_dirt.png"});
    TileSpriteBuffer.setShader(shader_handle);

    ShapeDisplayBuffer.init(MaxShapes);
    ShapeDisplayBuffer.setShader(loadShaderProgram("basic.vert", "basic.geom", "shape.frag"));
}

void WaywardGL::render() {
    Display.CameraX = 0;//-PlayerSpriteBuffer.vertices[0];
    Display.CameraY = -4000;//-PlayerSpriteBuffer.vertices[1];

    PlayerSpriteBuffer.render(Display);
    TileSpriteBuffer.render(Display);
    ShapeDisplayBuffer.render(Display);
}

void WaywardGL::deinit() {
    // Clean up resources
}

SpriteBuffer *WaywardGL::spriteBuffer() { return &PlayerSpriteBuffer; }
SpriteBuffer *WaywardGL::  tileBuffer() { return &TileSpriteBuffer; }
ShapeBuffer  *WaywardGL:: shapeBuffer() { return &ShapeDisplayBuffer; }
