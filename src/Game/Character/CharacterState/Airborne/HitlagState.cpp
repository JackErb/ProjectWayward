//
//  HitlagState.cpp
//  main
//
//  Created by Jack Erb on 6/8/20.
//

#include "HitlagState.hpp"
#include "HitstunState.hpp"
#include "../../Character.hpp"

void HitlagState::init(int f, fpoat angle, fpoat basekb, fpoat kbscale) {
    // Save knockback & angle
    data.angle = angle;
    data.kb = basekb + kbscale * (character->Percent() * fpoat(0,1000));
    
    // Calculate hitlag length
    data.frame = f;
}

void HitlagState::ProcessInput(const PlayerInput& input) {
    // TODO: SDI
}

void HitlagState::Tick() {
    data.frame--;
    if (data.frame == 0) {
        character->SetActionState(new HitstunState(character, data.angle, data.kb));
        return;
    }
}

void HitlagState::HandleCollision(const Entity &e, const VectorV pv) {

}

void HitlagState::SwitchState(CharState state) {
    // Do nothing, continue hitlag
}
