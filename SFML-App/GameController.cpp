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

GameController::GameController() : player_(0, {0.f, 0.f}), box_(1, {0.f, 600.f}) {
    player_.SetPolygons({{{0.f, 0.f}, {50.f, 0.f}, {50.f, 50.f}, {0.f, 50.f}}});
    player_.gravity = 2;
    player_.maxFallSpeed = 20;
    player_.SetState(CharacterState::AIRBORNE);
    
    sf::Texture *ptexture = new sf::Texture();
    textures_.push_back(ptexture);
    if (!ptexture->loadFromFile(resourcePath() + "player.png")) {
        // Couldn't load texture
        assert(false);
    }
    player_.SetSprite(sf::Sprite(*ptexture));
    
    box_.SetPolygons({{{0.f, 0.f}, {850.f, 0.f}, {850.f, 200.f}, {0.f, 200.f}}});
    box_.isStatic = true;
    
    sf::Texture *btexture = new sf::Texture();
    textures_.push_back(btexture);
    if (!btexture->loadFromFile(resourcePath() + "stage.png")) {
        // Couldn't load texture
        assert(false);
    }
    box_.SetSprite(sf::Sprite(*btexture));
    
    engine_ = PhysicsEngine();
    engine_.SetDelegate(this);
    engine_.AddEntity(&player_);
    engine_.AddEntity(&box_);
}

void GameController::Tick() {
    engine_.Update();
}

void GameController::ProcessInput(const PlayerInput &input) {
    player_.ProcessInput(input);
}

void GameController::Render(sf::RenderWindow *window) {
    window->draw(player_.Sprite());
    window->draw(box_.Sprite());
}

/* PhysicsDelegate methods */
void GameController::collision(Entity *e1, Entity *e2, const sf::Vector2f &push_vec) {
    if (e1 == &player_) {
        e1->Transform(push_vec);
        
        // Check that player is above stage
        player_.SetState(CharacterState::GROUNDED);
    }
}
