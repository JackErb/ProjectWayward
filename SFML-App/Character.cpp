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

Character::~Character() {
    if (actionState_ != nullptr) delete actionState_;
    if (cleanupState_ != nullptr) delete cleanupState_;
}

Character::Character(int id, sf::Vector2f vec) : Entity(id, vec), input_(nullptr) {
    SetActionState(new AirborneNeutralState(this));
}

void Character::ProcessInput(const PlayerInput &input) {
    input_ = const_cast<PlayerInput* const>(&input);
    actionState_->ProcessInput(input);
}

void Character::Tick() {
    if (Position().y > 1500) {
        SetPosition({Position().x, -1000});
    }
    
    if (cleanupState_ != nullptr) {
        delete cleanupState_;
        cleanupState_ = nullptr;
    }
    
    if (actionState_->GetState() == AIRBORNE &&
        fallthrough_ != nullptr)  {
        ftCount_--;
        if (ftCount_ == 0) fallthrough_ = nullptr;
    }

    if (actionState_->GetState() == GROUNDED && !engine->CheckBoundingBoxCollision(this, groundedData.stage)) {
        actionState_->SwitchState(AIRBORNE);
        initAirborneData();
    }
    
    actionState_->Tick();
}

void Character::HandleCollision(const Entity &entity, sf::Vector2f pv) {
    if (fallthrough_ != nullptr && &entity == fallthrough_) {
        return;
    }
    actionState_->HandleCollision(entity, pv);
}

int Character::Direction() const {
    if (actionState_->GetState() == AIRBORNE) {
        return airborneData.direction;
    } else {
        return groundedData.direction;
    }
}

void Character::Jump(JumpType type, bool fullhop) {
    if (actionState_->GetState() != AIRBORNE) {
        std::cerr << "ERROR: JUMP INVALID STATE " << std::endl;
        return;
    }
    
    if (airborneData.fastfall) {
        airborneData.fastfall = false;
    }
    
    float xv;
    float yv = fullhop ? attr.FullhopJump : attr.ShorthopJump;
    if (type == LEFT || type == UP || type == RIGHT) {
        initAirborneData();
    }
    switch (type) {
        case LEFT:
            xv = - attr.MaxGroundSpeed;
            break;
        case UP:
            xv = 0.f;
            break;
        case RIGHT:
            xv = attr.MaxGroundSpeed;
            break;
        case DJUMP:
            if (airborneData.jumps > 0) {
                airborneData.jumps--;
                xv = 0.f;
                yv = attr.DoubleJump;
            } else {
                // No jumps left
                return;
            }
            break;
    }
    SetActionState(new AirborneNeutralState(this));
    velocity_.y = yv;
    velocity_.x = xv;
}

void Character::Dash(float m) {
    if (actionState_->GetState() != GROUNDED) {
        std::cerr << "ERROR: DASH INVALID STATE " << std::endl;
        return;
    }
    
    if (abs(m) > 1.f) {
        std::cerr << "INVALID DASH MODIFIER" << std::endl;
        return;
    }
    
    groundedData.direction = m < 0 ? -1 : 1;
        
    velocity_.x = m * attr.MaxGroundSpeed;
}

void Character::Vector(float m) {
    if (actionState_->GetState() != AIRBORNE) {
        std::cerr << "ERROR: VECTOR INVALID STATE " << std::endl;
        return;
    }
    
    if (abs(m) > 1.f) {
        std::cerr << "INVALID VECTOR MODIFIER" << std::endl;
        return;
    }
    
    velocity_.x += m * attr.AirAccel;
    velocity_.x = (velocity_.x < 0 ? -1 : 1) * fmin(attr.MaxAirSpeed, abs(velocity_.x));
}

void Character::ApplyFriction() {
    velocity_.x *= (actionState_->GetState() == GROUNDED) ? attr.GroundFriction : attr.AirFriction;
}

void Character::SetActionState(CharacterState *s) {
    cleanupState_ = actionState_;
    actionState_ = s;
}

void Character::FallthroughPlatform() {
    if (actionState_->GetState() == AIRBORNE ||
        groundedData.stage->Type() != EntityType::PLATFORM) {
        std::cerr << "ERROR Attempt to fall through " << groundedData.stage->Type();
        std::cerr << ", WITH STATE " << actionState_->GetState() << std::endl;
        return;
    }
    
    fallthrough_ = groundedData.stage;
    ftCount_ = 4;
}

void Character::WallJump(int dir) {
    if (actionState_->GetState() != AIRBORNE) {
        std::cerr << "ERROR Attempt to walljump, state: " << actionState_->GetState() << std::endl;
        return;
    }
    
    if (airborneData.walljump) {
        velocity_.x = dir * attr.MaxAirSpeed / 1.5f;
        velocity_.y = attr.WallJump;
        airborneData.walljump = false;
    }
}

void Character::Airdodge() {
    if (actionState_->GetState() != AIRBORNE
        || !airborneData.airdodge) {
        std::cerr << "ERROR Attempt to airdodge, state: " << actionState_->GetState();
        std::cerr << ", airdodge: " << airborneData.airdodge << std::endl;
        return;
    }
    
    float a = input_->stick.angle();
    velocity_.x = cos(a) * attr.AirdodgeVelocity;
    velocity_.y = - sin(a) * attr.AirdodgeVelocity;
    airborneData.airdodge = false;
}
