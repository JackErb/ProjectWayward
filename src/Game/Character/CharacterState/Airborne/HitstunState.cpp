//
//  HitstunState.cpp
//  main
//
//  Created by Jack Erb on 6/8/20.
//

#include "HitstunState.hpp"
#include "AirborneNeutralState.hpp"
#include "../../Character.hpp"

void HitstunState::init(float angle, float kb) {
    character_->Knockback(angle, kb);
    data.frame_ = kb * 0.4;
}

void HitstunState::ProcessInput(const PlayerInput& input) {

}

void HitstunState::Tick() {
    character_->ApplyGravity();
    character_->ApplyFriction();
    character_->ApplyVelocity();
    
    data.frame_--;
    if (data.frame_ == 0) {
        character_->ClearHitboxIgnore();
        character_->SetActionState(new AirborneNeutralState(character_));
        return;
    }
}

void HitstunState::HandleCollision(const Entity &e, const sf::Vector2f pv) {

}

void HitstunState::SwitchState(CharState state) {

}
