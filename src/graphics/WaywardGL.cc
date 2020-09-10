#include "WaywardGL.h"

#include "resource_path.h"
#include "TextureLoader.h"
#include "Shader.h"
#include "SpriteBuffer.h"
#include "ShapeBuffer.h"
#include "MetaballBuffer.h"

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

static const int MaxTiles = 100000;
static SpriteBuffer TileSpriteBuffer;

static const int MaxShapes = 5000;
static ShapeBuffer ShapeDisplayBuffer;

static const int MaxWaterMetaballs = 100;
static MetaballBuffer WaterBuffer;

void WaywardGL::init(int width, int height) {
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);

    Display.WindowWidth = width;
    Display.WindowHeight = height;
    Display.WindowScale = 0.2;

    PlayerSpriteBuffer.init(MaxSprites, "PlayerSpriteBuffer",
                            {"jump_0.png", "jump_1.png", "jump_2.png", "jump_3.png",
                             "land_0.png", "land_1.png", "land_2.png", "land_3.png"});
    PlayerSpriteBuffer.setShader(loadShaderProgram("basic.vert", "basic.geom", "basic.frag"));

    TileSpriteBuffer.init(MaxTiles, "TileSpriteBuffer",
                          {"tile_grass.png", "tile_grass2.png",
                           "tile_sand.png", "tile_dirt.png"});
    TileSpriteBuffer.setShader(loadShaderProgram("tile.vert", "basic.geom", "basic.frag"));

    ShapeDisplayBuffer.init(MaxShapes);
    ShapeDisplayBuffer.setShader(loadShaderProgram("basic.vert", "basic.geom", "shape.frag"));

    WaterBuffer.init(MaxWaterMetaballs, "WaterBuffer");
    WaterBuffer.setShader(loadShaderProgram("metaball.vert", "metaball.frag"));
}

void WaywardGL::render() {
    Display.CameraX = -PlayerSpriteBuffer.vertices[0];
    Display.CameraY = -PlayerSpriteBuffer.vertices[1];

    PlayerSpriteBuffer.render(Display);
    TileSpriteBuffer.render(Display);
    ShapeDisplayBuffer.render(Display);
    WaterBuffer.render(Display);
}

void WaywardGL::deinit() {
    // Clean up resources
}

SpriteBuffer *WaywardGL::spriteBuffer() { return &PlayerSpriteBuffer; }
SpriteBuffer *WaywardGL::  tileBuffer() { return &TileSpriteBuffer; }
ShapeBuffer  *WaywardGL:: shapeBuffer() { return &ShapeDisplayBuffer; }
MetaballBuffer *WaywardGL::waterBuffer() { return &WaterBuffer; }
