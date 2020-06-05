//
//  OfflineGameController.cpp
//  SFML-App
//
//  Created by Jack Erb on 5/6/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#include <iostream>
#include <list>
#include <math.h>

#include <SFML/Graphics.hpp>

#include "OfflineGameController.hpp"
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

OfflineGameController::OfflineGameController(float w, float h) : player_(0, {-90.f, 0.f}), camera_(&entities_, w, h) {
    player_.polygons = {{{0.f, 0.f}, {175.f, 0.f}, {175.f, 250.f}, {0.f, 250.f}}};
    SpriteLoader::AnimationResult res = SpriteLoader::LoadAnimations({"dash"});
    player_.SetAnimMap(res.anims);
    player_.SetSprite(res.anims["dash"][0]);
    for (sf::Texture *text : res.textures_) {
        textures_.push_back(text);
    }
    
    StageEntity *stage = new StageEntity(1, {-1200.f, 300.f});
    stage->polygons = {{{0.f, 0.f}, {2400.f, 0.f}, {2400.f, 1000.f}, {0.f, 1000.f}}};
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
    
    PlatformEntity *platform = new PlatformEntity(3, {-300.f, -100.f});
    platform->polygons = {{{0.f, 0.f}, {600.f, 0.f}, {600.f, 20.f}, {0.f, 20.f}}};
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
    AddStage(stage);
    AddStage(platform);
}

void OfflineGameController::PreTick() {
    if (iindex == 240) {
        // Rollback testing
        RollbackAndReplay();
        iindex = 0;
    }
    if (iindex == 0) {
        RollbackTick();
    }
}

void OfflineGameController::Tick() {
    engine_.Update();
}

void OfflineGameController::ProcessInput(const PlayerInput &input, bool rb) {
    player_.ProcessInput(input);
    if (!rb) {
        inputs[iindex++] = input;
    }
}

void OfflineGameController::Render(sf::RenderWindow *window) {
    camera_.Tick();
    camera_.Render(window);
}

void OfflineGameController::Rollback() {
    // TODO: Rollback GameController's internal state
    
    // Roll back
    //cout << "Rolling back " << frames << " frames from frame " << network_.frame_ << endl;
    engine_.Rollback();
}

void OfflineGameController::RollbackAndReplay() {
    sf::Vector2f pos1 = player_.Position();
    Rollback();
    for (int i = 0; i < 240; i++) {
        ProcessInput(inputs[i], true);
        Tick();
    }
    sf::Vector2f pos2 = player_.Position();
    assert(fabs(pos1.x - pos2.x) < 0.01 && fabs(pos1.y - pos2.y) < 0.01);
}


