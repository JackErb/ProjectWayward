//
//  GameController.hpp
//  SFML-App
//
//  Created by Jack Erb on 4/24/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef GameController_hpp
#define GameController_hpp

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>

#include "PlayerInput.hpp"
#include "PhysicsEngine.hpp"
#include "Entity.hpp"
#include "Character.hpp"
#include "PlatformEntity.hpp"

class GameController {
public:
    GameController();
    
    void Tick();
    void ProcessInput(const PlayerInput &input);
    void Render(sf::RenderWindow *window);
        
private:
    Character player_;
    StageEntity stage_;
    PlatformEntity platform_;
    PhysicsEngine engine_;
    std::vector<sf::Texture*> textures_;
};

#endif /* GameController_hpp */
