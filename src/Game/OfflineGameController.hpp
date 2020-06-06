//
//  OfflineGameController.hpp
//  SFML-App
//
//  Created by Jack Erb on 5/6/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef OfflineGameController_hpp
#define OfflineGameController_hpp

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>

#include "PlayerInput.hpp"
#include "PhysicsEngine.hpp"
#include "Entity.hpp"
#include "Character.hpp"
#include "PlatformEntity.hpp"
#include "CameraController.hpp"
#include "OfflineNetworkController.hpp"

class OfflineGameController {
public:
    OfflineGameController(float w, float h);
    ~OfflineGameController() {
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
        
    void PreTick();
    void ProcessInput(const PlayerInput &input, bool rb = false);
    void Tick();
    void Render(sf::RenderWindow *window);
    
    void RollbackTick() {
        engine_.RollbackTick();
    }
    void Rollback();
    void RollbackAndReplay();
    
    void AddCharacter(Character *ch) {
        engine_.AddCharacter(ch);
        entities_.push_back(ch);
    }
    
    void AddStage(StageEntity *s) {
        engine_.AddStage(s);
        entities_.push_back(s);
    }
    
    PlayerInput inputs[240];
    int iindex = 0;
        
private:
    Character player_;
    
    PhysicsEngine engine_;
    CameraController camera_;
    
    std::vector<sf::Texture*> textures_;
    std::vector<sf::Sprite*> sprites_;
    std::vector<Entity*> entities_;
    
    OfflineNetworkController network_;

    friend class NetworkController;
};

#endif /* OfflineGameController_hpp */
