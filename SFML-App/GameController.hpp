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
#include "CameraController.hpp"

class GameController {
public:
    GameController(float w, float h);
    ~GameController() {
        for (sf::Texture *t : textures_) {
            delete t;
        }
        
        for (sf::Sprite* s : sprites_) {
            delete s;
        }
        
        for (Entity *e : entities_) {
            if (e != &player_) {
                delete e;
            }
        }
    }
    
    void Tick();
    void ProcessInput(const PlayerInput &input);
    void Render(sf::RenderWindow *window);
    
    void AddCharacter(Character *ch) {
        engine_.AddCharacter(ch);
        entities_.push_back(ch);
    }
    
    void AddStage(StageEntity *s) {
        engine_.AddStage(s);
        entities_.push_back(s);
    }
        
private:
    Character player_;
    PhysicsEngine engine_;
    CameraController camera_;
    std::vector<sf::Texture*> textures_;
    std::vector<sf::Sprite*> sprites_;
    std::vector<Entity*> entities_;
    
};

#endif /* GameController_hpp */
