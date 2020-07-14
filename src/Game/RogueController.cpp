//
//  GameController.cpp
//  SFML-App
//
//  Created by Jack Erb on 4/24/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#include "RogueController.hpp"

#include <iostream>
#include <list>
#include <string>
#include <math.h>

#if defined(__APPLE__)
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2_image/SDL_image.h>
#else
#include "SDL.h"
#include "SDL_render.h"
#include "SDL_image.h"
#endif

#include "PlayerInput.hpp"
#include "../Network/NetworkController.hpp"
#include "PlayerInput.hpp"
#include "Physics/PhysicsEngine.hpp"
#include "Entities/PlatformEntity.hpp"
#include "Entities/StageEntity.hpp"
#include "Loaders/SpriteLoader.hpp"
#include "Loaders/MoveLoader.hpp"
#include "Loaders/ResourcePath.hpp"
#include "../Generator/Generator.hpp"
#include "../Generator/LevelData.hpp"

using std::list;
using std::cout;
using std::cerr;
using std::endl;

RogueController::RogueController(SDL_Renderer *rd, float w, float h) : player_(0, { fpoat(0), fpoat(500,0,true) }), camera_(&engine_.entities_, w, h) {
	player_.polygons = {
        {{fpoat(40,0,true), fpoat(120,0)}, {fpoat(75,0), fpoat(120,0)},
		 {fpoat(75,0), fpoat(60,0,true)}, {fpoat(40,0,true), fpoat(60,0,true)}}//,
        //{{fpoat(40,0), fpoat(75,0,true)}, {fpoat(57,0), fpoat(0)}}
    };
    AnimMap res = SpriteLoader::LoadAnimations(rd, {{"dash", 10}}, 0.1515);
    player_.SetAnimMap(res);
    player_.SetTexture(res["dash"][0]);
    SpriteLoader::LoadTexture(rd, "rocket");
                  
    engine_.AddCharacter(&player_);
    GenerateRogueLevel();
}

void RogueController::GenerateRogueLevel() {
    GeneratorOptions opt;
    opt.mapWidth = 10;
    opt.mapHeight = 15;
    
    opt.chunkWidth = 10;
    opt.chunkHeight = 10;
    
    opt.tileWidth = 300;
    opt.tileHeight = 300;
    
    std::unordered_map<ChunkType, std::vector<ChunkTemplate>> templates;
    
    templates[Chunk_Main] = {
        ChunkTemplate(std::string("")
                      + "oooooooooo"
                      + "oooooooooo"
                      + "oooooooooo"
                      + "  ooooo   "
                      + "          "
                      + "          "
                      + "          "
                      + "oooooooooo"
                      + "oooooooooo"
                      + "oooooooooo"),
        ChunkTemplate(std::string("")
                      + "oooooooooo"
                      + "oooooooooo"
                      + "oooooooooo"
                      + "ooo       "
                      + "          "
                      + "          "
                      + "  ooooooo "
                      + "oooooooooo"
                      + "oooooooooo"
                      + "oooooooooo"),
        ChunkTemplate(std::string("")
        + "oooooooooo"
        + "oooooooooo"
        + "  oooooooo"
        + "      ooo"
        + "          "
        + "          "
        + "  oooo    "
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"),
        ChunkTemplate(std::string("")
                      + "oooooooooo"
                      + "oooooooooo"
                      + "oo     ooo"
                      + "          "
                      + "          "
                      + "          "
                      + "  oo   oo "
                      + "ooooo  ooo"
                      + "oooooooooo"
                      + "oooooooooo"),
        ChunkTemplate(std::string("")
        + "oooooooooo"
        + "oooooooooo"
        + "oooo   ooo"
        + "          "
        + "          "
        + "     oo   "
        + "   oooo   "
        + "o        o"
        + "oo       o"
        + "oooooooooo"),
        ChunkTemplate(std::string("")
        + "oooooooooo"
        + "ooo   oooo"
        + "o       oo"
        + "          "
        + "   ooooo  "
        + "  oooooo  "
        + "   oooo  o"
        + "o       oo"
        + "oooooooooo"
        + "oooooooooo"),
        ChunkTemplate(std::string("")
        + "oooooooooo"
        + "ooo   oooo"
        + "o       oo"
        + "          "
        + "   ooooo  "
        + " oooooooo "
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo")
    };
    
    templates[Chunk_MainDrop] = {
        ChunkTemplate(std::string("")
                      + "oooooooooo"
                      + "oooooooooo"
                      + "ooo   oooo"
                      + "o         "
                      + "          "
                      + "          "
                      + "oo        "
                      + "ooo    ooo"
                      + "ooo    ooo"
                      + "ooo    ooo"),
        ChunkTemplate(std::string("")
                      + "oooooooooo"
                      + "oooooooooo"
                      + "ooo     oo"
                      + "          "
                      + "          "
                      + "          "
                      + "          "
                      + "oo     ooo"
                      + "oo    oooo"
                      + "ooo   oooo"),
        ChunkTemplate(std::string("")
        + "oooooooooo"
        + "oooooooooo"
        + "ooo     oo"
        + "          "
        + "          "
        + "  ooo     "
        + "ooooo  oo   "
        + "ooo    ooo"
        + "ooo    ooo"
        + "ooo  ooooo")
    };
    
    templates[Chunk_MainFall] = {
        ChunkTemplate(std::string("")
                      + "ooo    ooo"
                      + "ooo    ooo"
                      + "ooo    ooo"
                      + "          "
                      + "   ooo    "
                      + "   oooo   "
                      + "          "
                      + "ooo    ooo"
                      + "ooo    ooo"
                      + "ooo    ooo"),
        ChunkTemplate(std::string("")
                    + "ooo    ooo"
                    + "ooo    ooo"
                    + "ooo    ooo"
                    + "oo      oo"
                    + "oo        "
                    + "oo        "
                    + "oo      oo"
                    + "ooo    ooo"
                    + "ooo    ooo"
                    + "ooo    ooo"),
        ChunkTemplate(std::string("")
        + "ooo    ooo"
        + "ooo    ooo"
        + "ooo    ooo"
        + "oo      oo"
        + "oooooo  oo"
        + "ooooo    o"
        + "oo      oo"
        + "ooo    ooo"
        + "ooo    ooo"
        + "ooo    ooo")
    };
    
    templates[Chunk_MainLand] = {
        ChunkTemplate(std::string("")
                      + "ooo    ooo"
                      + "oo     ooo"
                      + "o        o"
                      + "          "
                      + "          "
                      + "          "
                      + "   oooo   "
                      + "oooooooooo"
                      + "oooooooooo"
                      + "oooooooooo"),
        ChunkTemplate(std::string("")
                      + "ooo    ooo"
                      + "ooo    ooo"
                      + "ooo    ooo"
                      + "          "
                      + "    oo    "
                      + "   oooo   "
                      + "   ooooo  "
                      + "oooooooooo"
                      + "oooooooooo"
                      + "oooooooooo"),
        ChunkTemplate(std::string("")
        + "ooo    ooo"
        + "ooo    ooo"
        + "ooo    ooo"
        + "          "
        + "oooooo    "
        + "  ooooo   "
        + "   ooooo  "
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"),
        ChunkTemplate(std::string("")
        + "ooo    ooo"
        + "oo      oo"
        + "         o"
        + "          "
        + "   ooooo  "
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"),
    };
    
    templates[Chunk_Standard] = {
        ChunkTemplate(std::string("")
                      + "oooooooooo"
                      + "oooooooooo"
                      + "oooooooooo"
                      + "oooooooooo"
                      + "oooooooooo"
                      + "oooooooooo"
                      + "oooooooooo"
                      + "oooooooooo"
                      + "oooooooooo"
                      + "oooooooooo"),
        ChunkTemplate(std::string("")
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"),
        ChunkTemplate(std::string("")
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"),
        ChunkTemplate(std::string("")
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"),
        ChunkTemplate(std::string("")
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"),
        ChunkTemplate(std::string("")
        + "ooo    ooo"
        + "oo     ooo"
        + "o        o"
        + "          "
        + "          "
        + "          "
        + "   oooo   "
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"),
        ChunkTemplate(std::string("")
        + "oooooooooo"
        + "ooo    ooo"
        + "o        o"
        + "          "
        + "         o"
        + "        oo"
        + "        oo"
        + "oooo   ooo"
        + "ooo   oooo"
        + "oooooooooo"),
        ChunkTemplate(std::string("")
                      + "oooooooooo"
                      + "oooooooooo"
                      + "ooo   oooo"
                      + "          "
                      + "          "
                      + "          "
                      + "          "
                      + "ooo    ooo"
                      + "ooo    ooo"
                      + "ooo    ooo"),
        ChunkTemplate(std::string("")
                      + "oooooooooo"
                      + "oooooooooo"
                      + "ooo     oo"
                      + "          "
                      + "          "
                      + "          "
                      + "          "
                      + "oo    oooo"
                      + "oo    oooo"
                      + "ooo   oooo")
    };
    
    opt.chunkTemplates = &templates;
    level_ = GenerateLevel(opt);
    
    
    // Chunk dimensions in pixels
    int spawn_x = level_.chunks[level_.start_x][0].chunk_x + opt.chunkWidth * opt.tileWidth / 2;
    int spawn_y = opt.chunkHeight * opt.tileHeight / 2;
    
    player_.SetPosition({fpoat(spawn_x, 0), fpoat(spawn_y,0)});
    camera_.level = &level_;
    engine_.SetLevel(&level_);
}

void RogueController::PreTick(bool rb) {

}

void RogueController::Tick() {
    engine_.Update();
}

void RogueController::ProcessInput(const PlayerInput &pin, const PlayerInput &rin, bool rb) {
    player_.ProcessInput(pin);
}

void RogueController::Render(SDL_Renderer *window) {
    camera_.Tick();
    camera_.Render(window);
}

void RogueController::RollbackTick() {
    engine_.RollbackTick();
}

void RogueController::Rollback() {
    engine_.Rollback();
}

void RogueController::RollbackAndReplay() {

}


