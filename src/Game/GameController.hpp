//
//  GameController.hpp
//  SFML-App
//
//  Created by Jack Erb on 4/24/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef GameController_hpp
#define GameController_hpp

#include <vector>

#include "PlayerInput.hpp"
#include "Physics/PhysicsEngine.hpp"
#include "Entities/Entity.hpp"
#include "Character/Character.hpp"
#include "Entities/PlatformEntity.hpp"
#include "CameraController.hpp"
#include "../Network/NetworkController.hpp"

struct SDL_Renderer;

class GameController {
public:
    GameController(SDL_Renderer *rd, float w, float h);
    ~GameController() {}
    
    void PreTick(bool rb = false);
    
    void Tick();
    void ProcessInput(const PlayerInput &pin, const PlayerInput &rin, bool rb = false);
    void Render(SDL_Renderer *rd);
    
    void RollbackTick();
    void Rollback();
    void RollbackAndReplay();
        
private:
    Character player_;
    Character remotePlayer_;
    PhysicsEngine engine_;
    CameraController camera_;
    
    NetworkController network_;

    friend class NetworkController;
    friend int main(int, char const**);
};

#endif /* GameController_hpp */
