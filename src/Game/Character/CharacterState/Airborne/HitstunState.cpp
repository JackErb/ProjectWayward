//
//  HitstunState.cpp
//  main
//
//  Created by Jack Erb on 6/8/20.
//

#include "HitstunState.hpp"
#include "AirborneNeutralState.hpp"
#include "../../Character.hpp"

void HitstunState::init(fpoat angle, fpoat kb) {
    character->Knockback(angle, kb);
    data.frame = (kb * fpoat(0,4000)).i();
}

void HitstunState::ProcessInput(const PlayerInput& input) {

}

void HitstunState::Tick() {
    character->ApplyGravity();
    character->ApplyFriction();
    character->ApplyVelocity();
    
    data.frame--;
    if (data.frame == 0) {
        character->SetActionState(new AirborneNeutralState(character));
        return;
    }
}

void HitstunState::HandleCollision(const Entity &e, const VectorV pv) {

}

void HitstunState::SwitchState(CharState state) {

}
