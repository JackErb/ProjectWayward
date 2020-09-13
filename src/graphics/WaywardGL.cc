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

static const int MaxTiles = 100000;
static SpriteBuffer TileSpriteBuffer;

static const int MaxShapes = 5000;
static ShapeBuffer ShapeDisplayBuffer;

static const int MaxWaterMetaballs = 400;
static MetaballBuffer WaterBuffer;

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
    Display.WindowScale = 0.15;

    PlayerSpriteBuffer.init(MaxSprites, "PlayerSpriteBuffer", 
                            animationFileNames({{"jump", 4}, {"land", 4}, {"dash", 10}}));
    PlayerSpriteBuffer.setShader(loadShaderProgram("basic.vert", "basic.geom", "basic.frag"));

    TileSpriteBuffer.init(MaxTiles, "TileSpriteBuffer",
                          {"tile_grass.png", "tile_grass2.png",
                           "tile_sand.png", "tile_dirt.png"});
    TileSpriteBuffer.setShader(loadShaderProgram("tile.vert", "basic.geom", "basic.frag"));

    ShapeDisplayBuffer.init(MaxShapes);
    ShapeDisplayBuffer.setShader(loadShaderProgram("basic.vert", "basic.geom", "shape.frag"));

    WaterBuffer.init(MaxWaterMetaballs, "WaterBuffer");
    WaterBuffer.setShader(loadShaderProgram("metaball.vert", "metaball.frag"));

    cout << "Max " << GL_MAX_FRAGMENT_UNIFORM_COMPONENTS << endl;
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

SpriteBuffer *WaywardGL::spriteBuffer() { return &PlayerSpriteBuffer; }
SpriteBuffer *WaywardGL::  tileBuffer() { return &TileSpriteBuffer; }
ShapeBuffer  *WaywardGL:: shapeBuffer() { return &ShapeDisplayBuffer; }
MetaballBuffer *WaywardGL::waterBuffer() { return &WaterBuffer; }
