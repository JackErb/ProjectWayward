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

using std::list;

Character::Character(int id, sf::Vector2f vec) : Entity(id, vec), input_(nullptr) {
    SetActionState(new AirborneNeutralState(this));
}

Character::~Character() {
    // TODO: Deal with rollback_ and other dynamically allocated data
}

void Character::ProcessInput(const PlayerInput &input) {
    input_ = const_cast<PlayerInput* const>(&input);
    data.actionState_->ProcessInput(input);
}

void Character::Tick() {
    // Teleport back above stage
    if (Position().y > 1500) {
        SetPosition({Position().x, -1000});
    }

    // Do state dependent actions
    switch (data.actionState_->GetState()) {
        case AIRBORNE:
            if (data.fallthrough_ != nullptr)  {
                data.ftCount_--;
                if (data.ftCount_ == 0) data.fallthrough_ = nullptr;
            }
            
            break;
        case GROUNDED:
            if (!engine->CheckBoundingBoxCollision(this, data.groundedData.stage)) {
                data.actionState_->SwitchState(AIRBORNE);
                initAirborneData();
            }
            break;
    }
    
    data.actionState_->Tick();
}

void Character::RollbackTick() {
    Entity::RollbackTick();
    
    GameData *copy = new GameData();
    *copy = data;
    rollback_.push_front(copy);
    if (rollback_.size() > rbFrames_) {
        // Clean up old states
        GameData *old = rollback_.back();
        auto it = rollback_.end();
        it--;
        if (data.actionState_ != old->actionState_ && (*it)->actionState_ != old->actionState_) {
            delete old->actionState_;
        }
        delete old;
        rollback_.pop_back();
    }
    
    data.actionState_->RollbackTick();
}

void Character::Rollback() {
    Entity::Rollback();
    
    
    GameData *rollback = rollback_.front();
    rollback_.pop_front();
    
    data = *rollback_.front();
    data.actionState_->Rollback();
}

void Character::HandleCollision(const Entity &entity, sf::Vector2f pv) {
    if (data.fallthrough_ != nullptr && &entity == data.fallthrough_) {
        return;
    }
    
    if (data.actionState_->GetState() == AIRBORNE) {
        if (entity.Type() == STAGE) {
            // Apply the push vector to prevent overlap
            Transform(pv);
            
            if (pv.x == 0 && pv.y < 0 && data.velocity_.y > 0) {
                // Land on the stage
                NullVelocityY();
                data.actionState_->SwitchState(GROUNDED);
                data.groundedData.stage = dynamic_cast<const StageEntity*>(&entity);
                return;
            }
        } else if (entity.Type() == PLATFORM) {
            if (pv.x == 0 && pv.y < 0) {
                // The character collided with the platform. Check if the character
                // is above the platform and falling down
                Rectangle b = BoundingBox();
                float py = b.y + b.h;
                float vy = data.velocity_.y;
                if (vy > 0 && py - vy - 2.f < entity.Position().y) {
                    // Land on the platform
                    NullVelocityY();
                    // Apply the push vector to prevent overlap
                    Transform(pv);
                    
                    data.actionState_->SwitchState(GROUNDED);
                    data.groundedData.stage = dynamic_cast<const StageEntity*>(&entity);
                    return;
                }
            }
        }
    }
    
    data.actionState_->HandleCollision(entity, pv);
}

void Character::Jump(JumpType type, bool fullhop) {
    if (data.actionState_->GetState() != AIRBORNE) {
        std::cerr << "ERROR: JUMP INVALID STATE " << std::endl;
        return;
    }
    
    if (data.airborneData.fastfall) {
        data.airborneData.fastfall = false;
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
            if (data.airborneData.jumps > 0) {
                data.airborneData.jumps--;
                xv = 0.f;
                yv = attr.DoubleJump;
            } else {
                // No jumps left
                return;
            }
            break;
    }
    SetActionState(new AirborneNeutralState(this));
    data.velocity_.y = yv;
    data.velocity_.x = xv;
}

void Character::Dash(float m) {
    if (data.actionState_->GetState() != GROUNDED) {
        std::cerr << "ERROR: DASH INVALID STATE " << std::endl;
        return;
    }
    
    if (abs(m) > 1.f) {
        std::cerr << "INVALID DASH MODIFIER" << std::endl;
        return;
    }
    
    SetDirection(m < 0 ? -1 : 1);
        
    data.velocity_.x = m * attr.MaxGroundSpeed;
}

void Character::Vector(float m) {
    if (data.actionState_->GetState() != AIRBORNE) {
        std::cerr << "ERROR: VECTOR INVALID STATE " << std::endl;
        return;
    }
    
    if (abs(m) > 1.f) {
        std::cerr << "INVALID VECTOR MODIFIER" << std::endl;
        return;
    }
    
    data.velocity_.x += m * attr.AirAccel;
    data.velocity_.x = (data.velocity_.x < 0 ? -1 : 1) *
    fmin(attr.MaxAirSpeed, abs(data.velocity_.x));
}

void Character::ApplyFriction() {
    data.velocity_.x *= (data.actionState_->GetState() == GROUNDED) ?
            attr.GroundFriction : attr.AirFriction;
}

void Character::SetActionState(CharacterState *s) {
    data.actionState_ = s;
}

void Character::FallthroughPlatform() {
    if (data.actionState_->GetState() == AIRBORNE ||
        data.groundedData.stage->Type() != EntityType::PLATFORM) {
        std::cerr << "ERROR Attempt to fall through " << data.groundedData.stage->Type();
        std::cerr << ", WITH STATE " << data.actionState_->GetState() << std::endl;
        return;
    }
    
    data.fallthrough_ = data.groundedData.stage;
    data.ftCount_ = 4;
}

void Character::WallJump(int dir) {
    if (data.actionState_->GetState() != AIRBORNE) {
        std::cerr << "ERROR Attempt to walljump, state: " << data.actionState_->GetState() << std::endl;
        return;
    }
    
    if (data.airborneData.walljump) {
        data.velocity_.x = dir * attr.MaxAirSpeed / 1.5f;
        data.velocity_.y = attr.WallJump;
        data.airborneData.walljump = false;
    }
}

void Character::Airdodge() {
    if (data.actionState_->GetState() != AIRBORNE
        || !data.airborneData.airdodge) {
        std::cerr << "ERROR Attempt to airdodge, state: " << data.actionState_->GetState();
        std::cerr << ", airdodge: " << data.airborneData.airdodge << std::endl;
        return;
    }
    
    float a = input_->stick.angle();
    data.velocity_.x = cos(a) * attr.AirdodgeVelocity;
    data.velocity_.y = - sin(a) * attr.AirdodgeVelocity;
    data.airborneData.airdodge = false;
}
