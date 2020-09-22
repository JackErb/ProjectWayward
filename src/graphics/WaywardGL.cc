#include "WaywardGL.h"

#include "resource_path.h"
#include "TextureLoader.h"
#include "ShaderLoader.h"
#include "SpriteBuffer.h"
#include "ShapeBuffer.h"
#include "MetaballBuffer.h"

#include <glad/glad.h>
#include <string>
#include <iostream>
#include <vector>
#include <utility>

using namespace WaywardGL;

using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::pair;

static DisplayData Display;

static const int MaxSprites = 100;
static SpriteBuffer PlayerSpriteBuffer;

static const int MaxTiles = 1000000;
static SpriteBuffer TileSpriteBuffer;

static const int MaxShapes = 5000;
static ShapeBuffer ShapeDisplayBuffer;

static const int MaxWaterMetaballs = 400;
static MetaballBuffer WaterBuffer;

static SpriteBuffer GenericSpriteBuffer;

vector<string> animationFileNames(vector<pair<string, int>> files) {
    vector<string> res;
    for (auto pair : files) {
        string name = pair.first;
        int count = pair.second;
        for (int i = 0; i < count; i++) {
            string file_name = name + "_" + std::to_string(i) + ".png";
            res.push_back(file_name);
        }
    }
    return res;
}

void WaywardGL::init(int width, int height) {
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);

    Display.WindowWidth = width;
    Display.WindowHeight = height;
    Display.WindowScale = 0.3;

    unsigned int basicShaderProg = loadShaderProgram("basic.vert", "basic.geom", "basic.frag");

    unsigned int player_handle = loadTextures(
        animationFileNames(
              {{"jump", 4}, {"land", 4}, {"dash", 10}}
        ));
    PlayerSpriteBuffer.init(MaxSprites, "PlayerSpriteBuffer", player_handle, true);
    PlayerSpriteBuffer.setShader(basicShaderProg);

    unsigned int tile_handle = loadTextures({"tile_grass.png", "tile_grass2.png",
                                             "tile_sand.png", "tile_dirt.png"});
    TileSpriteBuffer.init(MaxTiles, "TileSpriteBuffer", tile_handle, true);
    TileSpriteBuffer.setShader(loadShaderProgram("tile.vert", "basic.geom", "basic.frag"));

    ShapeDisplayBuffer.init(MaxShapes);
    ShapeDisplayBuffer.setShader(loadShaderProgram("basic.vert", "basic.geom", "shape.frag"));

    WaterBuffer.init(MaxWaterMetaballs, "WaterBuffer");
    WaterBuffer.setShader(loadShaderProgram("metaball.vert", "metaball.frag"));

    GenericSpriteBuffer.init(1, "GenericSpriteBuffer", -1, false);
    GenericSpriteBuffer.setShader(loadShaderProgram("basic.vert", "basic.geom", "basic-noatlas.frag"));
    GenericSpriteBuffer.addSprite(0,0,0,0,0);
}

void WaywardGL::render() {
    Display.CameraX = -PlayerSpriteBuffer.vertices[0];
    Display.CameraY = -PlayerSpriteBuffer.vertices[1];

    ShapeDisplayBuffer.render(Display);
    PlayerSpriteBuffer.render(Display);
    WaterBuffer.render(Display);
    TileSpriteBuffer.render(Display);
}

void WaywardGL::deinit() {
    // Clean up resources
}

void WaywardGL::renderSprite(float x, float y, float w, float h, unsigned int texture_handle) {
    GenericSpriteBuffer.setSpritePos(0, x, y);
    GenericSpriteBuffer.setSpriteSize(0, w, h);
    GenericSpriteBuffer.setTextureHandle(texture_handle);

    GenericSpriteBuffer.render(Display);
}

SpriteBuffer *WaywardGL::spriteBuffer() { return &PlayerSpriteBuffer; }
SpriteBuffer *WaywardGL::  tileBuffer() { return &TileSpriteBuffer; }
ShapeBuffer  *WaywardGL:: shapeBuffer() { return &ShapeDisplayBuffer; }
MetaballBuffer *WaywardGL::waterBuffer() { return &WaterBuffer; }
