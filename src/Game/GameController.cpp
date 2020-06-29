//
//  GameController.cpp
//  SFML-App
//
//  Created by Jack Erb on 4/24/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

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

#include "GameController.hpp"
#include "PlayerInput.hpp"
#include "Physics/PhysicsEngine.hpp"
#include "Entities/PlatformEntity.hpp"
#include "Entities/StageEntity.hpp"
#include "Loaders/SpriteLoader.hpp"
#include "Loaders/MoveLoader.hpp"
#include "Loaders/ResourcePath.hpp"

using std::list;
using std::cout;
using std::cerr;
using std::endl;

GameController::GameController(SDL_Renderer *rd, float w, float h) : player_(0, { fpoat(0), fpoat(500,0,true) }),
		remotePlayer_(5, { fpoat(0), fpoat(500,0,true) }), camera_(&engine_.entities_, w, h) {
	cout << "HERE!!!!" << endl;
	player_.polygons = {
        {{fpoat(40,0,true), fpoat(120,0)}, {fpoat(75,0), fpoat(120,0)},
		 {fpoat(75,0), fpoat(60,0,true)}, {fpoat(40,0,true), fpoat(60,0,true)}},
        {{fpoat(40,0), fpoat(75,0,true)}, {fpoat(57,0), fpoat(0)}}
    };
    AnimMap res = SpriteLoader::LoadAnimations(rd, {{"dash", 10}}, 0.1515);
    player_.SetAnimMap(res);
    player_.SetTexture(res["dash"][0]);

    remotePlayer_.polygons = {
		{{fpoat(40,0,true), fpoat(120,0)}, {fpoat(75,0), fpoat(120,0)},
		 {fpoat(75,0), fpoat(60,0,true)}, {fpoat(40,0,true), fpoat(60,0,true)}},
		{{fpoat(40,0), fpoat(75,0,true)}, {fpoat(57,0), fpoat(0)}}
	};
    AnimMap res2 = SpriteLoader::LoadAnimations(rd, {{"dash", 10}}, 0.1515);
    remotePlayer_.SetAnimMap(res2);
    remotePlayer_.SetTexture(res2["dash"][0]);
    
    StageEntity *stage = new StageEntity(1, {fpoat(0), fpoat(800,0)});
    stage->polygons = {
		{{fpoat(1200,0,true),fpoat(500,0,true)}, {fpoat(1200,0),fpoat(500,0,true)},
		 {fpoat(1200,0),fpoat(500,0)}, {fpoat(1200,0,true),fpoat(500,0)}}
    };
    stage->isStatic = true;
    stage->SetTexture(SpriteLoader::LoadTexture(rd, "stage"));
    
    PlatformEntity *platform = new PlatformEntity(3, {fpoat(0), fpoat(60,0,true)});
	platform->polygons = {
		{{fpoat(300,0,true),fpoat(10,0,true)}, {fpoat(300,0),fpoat(10,0,true)},
		 {fpoat(300,0),fpoat(10,0)}, {fpoat(-300,0,true),fpoat(10,0)}}
	};
    platform->isStatic = true;
    platform->SetTexture(SpriteLoader::LoadTexture(rd, "platform"));
                  
    engine_.AddCharacter(&player_);
    engine_.AddCharacter(&remotePlayer_);
    engine_.AddStage(stage);
    engine_.AddStage(platform);
}

void GameController::PreTick(bool rb) {
    if (!rb && network_.IsConnected()) {
        network_.PreTick();
        if (network_.frame_ == 0) {
            player_.SetPosition({fpoat(0), fpoat(0)});
            remotePlayer_.SetPosition({fpoat(0),fpoat(0)});
            RollbackTick();
        } else {
            RollbackAndReplay();
        }
    }
}

void GameController::Tick() {
    engine_.Update();
}

void GameController::ProcessInput(const PlayerInput &pin, const PlayerInput &rin, bool rb) {
    player_.ProcessInput(pin);
    remotePlayer_.ProcessInput(rin);
    
    if (!rb) {
        network_.SendPlayerInput(pin);
    }
}

void GameController::Render(SDL_Renderer *window) {
    camera_.Tick();
    camera_.Render(window);
}

void GameController::RollbackTick() {
    engine_.RollbackTick();
}

void GameController::Rollback() {
    engine_.Rollback();
}

void GameController::RollbackAndReplay() {
    VectorV pos1 = player_.Position();
    
    Rollback();
    
    // TODO: Replay
    std::list<NetworkController::InputData> *ls = network_.GetInputData();
    int i = 0;
    
    // This is guaranteed to be valid
    PlayerInput lastRemoteInput = ls->front().remote;
    for (auto inputData : *ls) {
        if (i == 1 && network_.frame_ >= NetworkController::RollbackFrames) {
            RollbackTick();
        }
        PreTick(true);
        if (inputData.isRemoteValid) {
            lastRemoteInput = inputData.remote;
        } else {
            inputData.remote = lastRemoteInput;
        }
        ProcessInput(inputData.player, inputData.remote, true);
        Tick();
        i++;
    }
    
    if (!network_.inputData_[network_.localFrameIndex_].isRemoteValid) {
        network_.inputData_[network_.localFrameIndex_].remote = lastRemoteInput;
    }
        
    delete ls;
    
}


