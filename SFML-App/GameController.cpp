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
    
    /*BlankEntity *b = new BlankEntity(4, {-200.f, 0.f});
    b->SetPolygons({{{0.f,0.f}, {40.f, 0.f}}});*/
                  
    engine_ = PhysicsEngine();
    AddCharacter(&player_);
    AddCharacter(&remotePlayer_);
    AddStage(stage);
    AddStage(platform);
}

void GameController::Tick() {
    engine_.Update();
}

void GameController::ProcessInput(const PlayerInput &input, bool rb, int f) {
    if (!rb) {
        int f = network_.CheckForRemoteInput();
        cout << "F: " << f << " " << network_.frame_ << endl;
        if (f != -1 && f < network_.frame_) {
            // A new frame was received; rollback to that point and replay
            int backtrack = network_.frame_ - f;
            Rollback(backtrack);
        }
        
        
        network_.SendInput(input);
    }

    if (!rb) {
        remotePlayer_.ProcessInput(network_.GetInput(network_.frame_));
        playerInputs_[++inputsIndex_ % NetworkController::RollbackFrames] = input;
    } else {
        remotePlayer_.ProcessInput(network_.GetInput(f));
    }
    player_.ProcessInput(input);
}

void GameController::Render(sf::RenderWindow *window) {
    camera_.Tick();
    camera_.Render(window);
}

void GameController::Rollback(int frames) {
    // TODO: Rollback GameController's internal state
    
    // Roll back
    cout << "Rolling back " << frames << " frames from frame " << network_.frame_ << endl;
    engine_.Rollback(frames);
    
    // Replay forward
    cout << "Replaying " << frames << " frames" << endl;
    int playerInputIndex = inputsIndex_ - frames;
    for (int i = 0; i < frames-1; i++) {
        if (playerInputIndex < 0) {
            playerInputIndex += NetworkController::RollbackFrames;
        } else if (playerInputIndex >= NetworkController::RollbackFrames) {
            playerInputIndex -= NetworkController::RollbackFrames;
        }
        ProcessInput(playerInputs_[playerInputIndex], true, network_.frame_ - frames + i);
        Tick();
        playerInputIndex++;
    }
}


