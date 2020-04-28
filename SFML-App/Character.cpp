//
//  Character.cpp
//  SFML-App
//
//  Created by Jack Erb on 4/26/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#include "Character.hpp"
#include "NeutralState.hpp"
#include "PhysicsEngine.hpp"
#include "AirborneNeutralState.hpp"

Character::Character(int id, sf::Vector2f vec) : Entity(id, vec) {
    SetActionState(new AirborneNeutralState(this));
}

void Character::ProcessInput(const PlayerInput &input) {
    actionState_->ProcessInput(input);
}

void Character::Tick() {
    if (cleanupState_ != nullptr) {
        delete cleanupState_;
        cleanupState_ = nullptr;
    }
    
    if (actionState_->GetState() == CharacterState::GROUNDED && !engine->CheckBoundingBoxCollisionWithStage(this)) {
        actionState_->SwitchState(AirborneNeutralState::AIRBORNE);
    }

    actionState_->Tick();
}

void Character::HandleCollision(const Entity &entity, sf::Vector2f pv) {
    actionState_->HandleCollision(entity, pv);
}

void Character::Jump(JumpType type, bool fullhop) {
    if (actionState_->GetState() != CharacterState::AIRBORNE) {
        std::cerr << "ERROR: JUMP INVALID STATE " << std::endl;
        return;
    }
    
    float xv;
    switch (type) {
        case LEFT:
            xv = -MaxGroundSpeed;
            break;
        case UP:
            xv = 0.f;
            break;
        case RIGHT:
            xv = MaxGroundSpeed;
            break;
        case DJUMP:
            xv = velocity_.x;
            break;
    }
    // TODO: JumpState
    SetActionState(new AirborneNeutralState(this));
    velocity_.y = JumpSpeed;
    velocity_.x = xv;
}

void Character::Dash(float m) {
    if (actionState_->GetState() != CharacterState::GROUNDED) {
        std::cerr << "ERROR: DASH INVALID STATE " << std::endl;
        return;
    }
    
    velocity_.x = m * MaxGroundSpeed;
}

void Character::Vector(float m) {
    if (actionState_->GetState() != CharacterState::AIRBORNE) {
        std::cerr << "ERROR: VECTOR INVALID STATE " << std::endl;
        return;
    }
    
    velocity_.x += m * AirAccel;
    velocity_.x = (velocity_.x < 0 ? -1 : 1) * fmin(MaxAirSpeed, abs(velocity_.x));
}

void Character::ApplyFriction() {
    velocity_.x *= (actionState_->GetState() == CharacterState::GROUNDED) ? GroundFriction : AirFriction;
}

void Character::SetActionState(CharacterState *s) {
    cleanupState_ = actionState_;
    actionState_ = s;
    std::cout << "STATE: " << s->GetState() << std::endl;
}
