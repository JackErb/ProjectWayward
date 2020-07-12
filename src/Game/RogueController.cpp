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
		 {fpoat(75,0), fpoat(60,0,true)}, {fpoat(40,0,true), fpoat(60,0,true)}},
        {{fpoat(40,0), fpoat(75,0,true)}, {fpoat(57,0), fpoat(0)}}
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
    level_ = GenerateLevel(opt);
    player_.SetPosition({fpoat(level_.start_x * 1600, 0), fpoat(400,0)});
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


