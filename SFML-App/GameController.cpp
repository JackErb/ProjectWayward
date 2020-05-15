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

#include <SFML/Graphics.hpp>

#include "GameController.hpp"
#include "PlayerInput.hpp"
#include "PhysicsEngine.hpp"
#include "Entity.hpp"
#include "ResourcePath.hpp"
#include "SpriteLoader.hpp"
#include "BlankEntity.hpp"

using std::list;
using std::cout;
using std::cerr;
using std::endl;

GameController::GameController(float w, float h) : player_(0, {-90.f, 0.f}), remotePlayer_(5, {-90.f, 0.f}), camera_(&entities_, w, h) {
    player_.SetPolygons({{{0.f, 0.f}, {175.f, 0.f}, {175.f, 250.f}, {0.f, 250.f}}});
    SpriteLoader::AnimationResult res = SpriteLoader::LoadAnimations({"dash"});
    player_.SetAnimMap(res.anims);
    player_.SetSprite(res.anims["dash"][0]);
    for (sf::Texture *text : res.textures_) {
        textures_.push_back(text);
    }
    
    remotePlayer_.SetPolygons({{{0.f, 0.f}, {175.f, 0.f}, {175.f, 250.f}, {0.f, 250.f}}});
    SpriteLoader::AnimationResult res2 = SpriteLoader::LoadAnimations({"dash"});
    remotePlayer_.SetAnimMap(res2.anims);
    remotePlayer_.SetSprite(res2.anims["dash"][0]);
    
    StageEntity *stage = new StageEntity(1, {-1200.f, 300.f});
    stage->SetPolygons({{{0.f, 0.f}, {2400.f, 0.f}, {2400.f, 1000.f}, {0.f, 1000.f}}});
    stage->isStatic = true;
    sf::Texture *btexture = new sf::Texture();
    textures_.push_back(btexture);
    if (!btexture->loadFromFile(resourcePath() + "stage.png")) {
        // Couldn't load texture
        assert(false);
    }
    sf::Sprite *s1 = new sf::Sprite(*btexture);
    sprites_.push_back(s1);
    stage->SetSprite(s1);
    
    PlatformEntity *platform = new PlatformEntity(3, {-300.f, -75.f});
    platform->SetPolygons({{{0.f, 0.f}, {600.f, 0.f}, {600.f, 20.f}, {0.f, 20.f}}});
    platform->isStatic = true;
    sf::Texture *pltexture = new sf::Texture();
    textures_.push_back(pltexture);
    if (!pltexture->loadFromFile(resourcePath() + "platform.png")) {
        // Couldn't load texture
        assert(false);
    }
    sf::Sprite *s2 = new sf::Sprite(*pltexture);
    sprites_.push_back(s2);
    platform->SetSprite(s2);
                  
    engine_ = PhysicsEngine();
    AddCharacter(&player_);
    AddCharacter(&remotePlayer_);
    AddStage(stage);
    AddStage(platform);
}

void GameController::PreTick(bool rb) {
    if (!rb) {
        network_.PreTick();
        if (network_.frame_ == 0) {
            player_.SetPosition({-90.f, 0});
            remotePlayer_.SetPosition({-90.f,0});
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

void GameController::Render(sf::RenderWindow *window) {
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
    sf::Vector2f pos1 = player_.Position();
    
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
        PlayerInput remote;
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
    
    sf::Vector2f pos2 = player_.Position();
    assert(fabs(pos1.x - pos2.x) < 0.01 && fabs(pos1.y - pos2.y) < 0.01);
}


