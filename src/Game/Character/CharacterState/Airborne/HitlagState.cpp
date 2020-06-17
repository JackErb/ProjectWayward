//
//  HitlagState.cpp
//  main
//
//  Created by Jack Erb on 6/8/20.
//

#include "HitlagState.hpp"
#include "HitstunState.hpp"
#include "../../Character.hpp"

void HitlagState::init(int f, float angle, float basekb, float kbscale) {
    // Save knockback & angle
    data.angle_ = angle;
    data.kb_ = basekb + kbscale * (character_->Percent() / 100.f);
    
    // Calculate hitlag length
    data.frame_ = f;
}

void HitlagState::ProcessInput(const PlayerInput& input) {
    // TODO: SDI
}

void HitlagState::Tick() {
    data.frame_--;
    if (data.frame_ == 0) {
        character_->SetActionState(new HitstunState(character_, data.angle_, data.kb_));
        return;
    }
}

void HitlagState::HandleCollision(const Entity &e, const VectorV pv) {

}

void HitlagState::SwitchState(CharState state) {
    // Do nothing, continue hitlag
}
