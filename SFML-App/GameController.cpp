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

GameController::GameController() : player_(0, {1100.f, 0.f}), stage_(1, {400.f, 1000.f}),
platform_(2, {850.f, 750.f}) {
    player_.SetPolygons({{{0.f, 0.f}, {100.f, 0.f}, {100.f, 100.f}, {0.f, 100.f}}});
    sf::Texture *ptexture = new sf::Texture();
    textures_.push_back(ptexture);
    if (!ptexture->loadFromFile(resourcePath() + "player.png")) {
        // Couldn't load texture
        assert(false);
    }
    player_.SetSprite(sf::Sprite(*ptexture));
    
    stage_.SetPolygons({{{0.f, 0.f}, {1400.f, 0.f}, {1400.f, 329.f}, {0.f, 329.f}}});
    stage_.isStatic = true;
    sf::Texture *btexture = new sf::Texture();
    textures_.push_back(btexture);
    if (!btexture->loadFromFile(resourcePath() + "stage.png")) {
        // Couldn't load texture
        assert(false);
    }
    stage_.SetSprite(sf::Sprite(*btexture));
    
    platform_.SetPolygons({{{0.f, 0.f}, {500.f, 0.f}, {500.f, 20.f}, {0.f, 20.f}}});
    platform_.isStatic = true;
    sf::Texture *pltexture = new sf::Texture();
    textures_.push_back(pltexture);
    if (!pltexture->loadFromFile(resourcePath() + "platform.png")) {
        // Couldn't load texture
        assert(false);
    }
    platform_.SetSprite(sf::Sprite(*pltexture));
                  
    engine_ = PhysicsEngine();
    engine_.AddCharacter(&player_);
    engine_.AddStage(&stage_);
    engine_.AddStage(&platform_);
}

void GameController::Tick() {
    engine_.Update();
}

void GameController::ProcessInput(const PlayerInput &input) {
    player_.ProcessInput(input);
}

void GameController::Render(sf::RenderWindow *window) {
    window->draw(platform_.Sprite());
    window->draw(player_.Sprite());
    window->draw(stage_.Sprite());
}
