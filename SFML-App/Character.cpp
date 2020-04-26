//
//  Character.cpp
//  SFML-App
//
//  Created by Jack Erb on 4/26/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#include "Character.hpp"
#include "NeutralState.hpp"
#include "JumpsquatState.hpp"

Character::Character(int id, sf::Vector2f vec) : Entity(id, vec) {
    SetState(AIRBORNE);
    actionState_ = new NeutralState(this);
}

void Character::ProcessInput(const PlayerInput &input) {
    actionState_->ProcessInput(input);
}

void Character::Tick() {
    if (cleanupState_ != nullptr) {
        delete cleanupState_;
        cleanupState_ = nullptr;
    }
    
    if (state_ == GROUNDED && !PhysicsEngine::intersects(*this, *stage_)) {
        SetState(AIRBORNE);
    }
    
    actionState_->Tick();
}

void Character::HandleCollision(const Entity &entity, sf::Vector2f pv) {
    actionState_->HandleCollision(entity, pv);
}


void Character::Jumpsquat() {
    if (state_ != GROUNDED) return;
    
    setActionState(new JumpsquatState(this));
}

void Character::Jump() {
    switch (state_) {
        case GROUNDED:
            setActionState(new JumpsquatState(this));
            break;
        case AIRBORNE:
            setActionState(new NeutralState(this));
            velocity.y = -40;
            break;
    }
}
