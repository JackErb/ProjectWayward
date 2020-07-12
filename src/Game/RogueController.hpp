//
//  RogueController.hpp
//  ProjectWayward
//
//  Created by Jack Erb on 7/11/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef RogueController_hpp
#define RogueController_hpp

#include "AbstractGameController.hpp"

#include <vector>

#include "../Network/NetworkController.hpp"
#include "Physics/PhysicsEngine.hpp"
#include "Entities/Entity.hpp"
#include "Character/Character.hpp"
#include "Entities/PlatformEntity.hpp"
#include "CameraController.hpp"
#include "../Generator/LevelData.hpp"

struct SDL_Renderer;
class PlayerInput;

class RogueController: public AbstractGameController {
public:
    RogueController(SDL_Renderer *rd, float w, float h);
    ~RogueController() {}
        
    void PreTick(bool rb = false);
    
    void Tick();
    void ProcessInput(const PlayerInput &pin, const PlayerInput &rin, bool rb = false);
    void Render(SDL_Renderer *rd);
    
    void RollbackTick();
    void Rollback();
    void RollbackAndReplay();
        
private:
    void GenerateRogueLevel();
    
private:
    Character player_;
    PhysicsEngine engine_;
    CameraController camera_;
    
    LevelData level_;
    
    friend class NetworkController;
    friend int main(int, char**);
};

#endif /* GameController_hpp */
