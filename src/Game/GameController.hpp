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
#include "Physics/PhysicsEngine.hpp"
#include "Entities/Entity.hpp"
#include "Character/Character.hpp"
#include "Entities/PlatformEntity.hpp"
#include "CameraController.hpp"
#include "../Network/NetworkController.hpp"

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
    }
    
    void PreTick(bool rb = false);
    
    void Tick();
    void ProcessInput(const PlayerInput &pin, const PlayerInput &rin, bool rb = false);
    void Render(sf::RenderWindow *window);
    
    void RollbackTick();
    void Rollback();
    void RollbackAndReplay();
        
private:
    Character player_;
    Character remotePlayer_;
    PhysicsEngine engine_;
    CameraController camera_;
    std::vector<sf::Texture*> textures_;
    std::vector<sf::Sprite*> sprites_;
    
    NetworkController network_;

    friend class NetworkController;
    friend int main(int, char const**);
};

#endif /* GameController_hpp */
