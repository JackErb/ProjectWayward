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
#include "LandingLagState.hpp"
#include "StageEntity.hpp"
#include "PlatformEntity.hpp"

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
    // Teleport back above stage
    if (Position().y > 1500) {
        SetPosition({Position().x, -1000});
    }
    
    // Cleanup old state
    if (cleanupState_ != nullptr) {
        delete cleanupState_;
        cleanupState_ = nullptr;
    }

    // Do state dependent actions
    switch (actionState_->GetState()) {
        case AIRBORNE:
            if (fallthrough_ != nullptr)  {
                ftCount_--;
                if (ftCount_ == 0) fallthrough_ = nullptr;
            }
            
            break;
        case GROUNDED:
            if (!engine->CheckBoundingBoxCollision(this, groundedData.stage)) {
                actionState_->SwitchState(AIRBORNE);
                initAirborneData();
            }
            break;
    }
    
    actionState_->Tick();
}

void Character::HandleCollision(const Entity &entity, sf::Vector2f pv) {
    if (fallthrough_ != nullptr && &entity == fallthrough_) {
        return;
    }
    
    if (actionState_->GetState() == AIRBORNE) {
        if (entity.Type() == STAGE) {
            // Apply the push vector to prevent overlap
            Transform(pv);
            
            if (pv.x == 0 && pv.y < 0 && velocity_.y > 0) {
                // Land on the stage
                NullVelocityY();
                actionState_->SwitchState(GROUNDED);
                groundedData.stage = dynamic_cast<const StageEntity*>(&entity);
                return;
            }
        } else if (entity.Type() == PLATFORM) {
            if (pv.x == 0 && pv.y < 0) {
                // The character collided with the platform. Check if the character
                // is above the platform and falling down
                Rectangle b = BoundingBox();
                float py = b.y + b.h;
                if (velocity_.y > 0 && py - velocity_.y - 2.f < entity.Position().y) {
                    // Land on the platform
                    NullVelocityY();
                    // Apply the push vector to prevent overlap
                    Transform(pv);
                    
                    actionState_->SwitchState(GROUNDED);
                    groundedData.stage = dynamic_cast<const StageEntity*>(&entity);
                    return;
                }
            }
        }
    }
    
    actionState_->HandleCollision(entity, pv);
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
    
    SetDirection(m < 0 ? -1 : 1);
        
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
