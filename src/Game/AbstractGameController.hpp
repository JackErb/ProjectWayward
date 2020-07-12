//
//  AbstractGameController.hpp
//  ProjectWayward
//
//  Created by Jack Erb on 7/11/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef AbstractGameController_hpp
#define AbstractGameController_hpp

class PlayerInput;
struct SDL_Renderer;

class AbstractGameController {
public:
    virtual void PreTick(bool rb) = 0;
    virtual void ProcessInput(const PlayerInput &pin, const PlayerInput &rin, bool rb) = 0;
    virtual void Tick() = 0;
    virtual void Render(SDL_Renderer *rd) = 0;
    
    virtual void RollbackTick() = 0;
    virtual void Rollback() = 0;
};

#endif  // AbstractGameController_hpp
