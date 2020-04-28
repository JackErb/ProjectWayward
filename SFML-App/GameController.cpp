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

using std::list;
using std::cout;
using std::cerr;
using std::endl;

GameController::GameController(float w, float h) : player_(0, {-50.f, -500.f}), stage_(1, {-700.f, 0.f}),
platform_(2, {-250.f, -200.f}), camera_(&entities_, w, h) {
    player_.SetPolygons({{{0.f, 0.f}, {100.f, 0.f}, {100.f, 100.f}, {0.f, 100.f}}});
    sf::Texture *ptexture = new sf::Texture();
    textures_.push_back(ptexture);
    if (!ptexture->loadFromFile(resourcePath() + "player.png")) {
        // Couldn't load texture
        assert(false);
    }
    sf::Sprite *s0 = new sf::Sprite(*ptexture);
    sprites_.push_back(s0);
    player_.SetSprite(s0);
    
    stage_.SetPolygons({{{0.f, 0.f}, {1400.f, 0.f}, {1400.f, 329.f}, {0.f, 329.f}}});
    stage_.isStatic = true;
    sf::Texture *btexture = new sf::Texture();
    textures_.push_back(btexture);
    if (!btexture->loadFromFile(resourcePath() + "stage.png")) {
        // Couldn't load texture
        assert(false);
    }
    sf::Sprite *s1 = new sf::Sprite(*btexture);
    sprites_.push_back(s1);
    stage_.SetSprite(s1);
    
    platform_.SetPolygons({{{0.f, 0.f}, {500.f, 0.f}, {500.f, 20.f}, {0.f, 20.f}}});
    platform_.isStatic = true;
    sf::Texture *pltexture = new sf::Texture();
    textures_.push_back(pltexture);
    if (!pltexture->loadFromFile(resourcePath() + "platform.png")) {
        // Couldn't load texture
        assert(false);
    }
    sf::Sprite *s2 = new sf::Sprite(*pltexture);
    sprites_.push_back(s2);
    platform_.SetSprite(s2);
                  
    engine_ = PhysicsEngine();
    AddCharacter(&player_);
    AddStage(&stage_);
    AddStage(&platform_);
}

void GameController::Tick() {
    engine_.Update();
}

void GameController::ProcessInput(const PlayerInput &input) {
    player_.ProcessInput(input);
}

void GameController::Render(sf::RenderWindow *window) {
    camera_.Render(window);
}
