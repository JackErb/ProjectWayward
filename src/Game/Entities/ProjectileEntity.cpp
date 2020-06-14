//
//  ProjectileEntity.cpp
//  main
//
//  Created by Jack Erb on 6/11/20.
//

#include "ProjectileEntity.hpp"
#include "../../MSL/MslInterpreter.hpp"
#include "../Loaders/SpriteLoader.hpp"

#include <string>

void ProjectileEntity::init(std::string move) {
    // TODO: Deal with texture
    SetMoveScript(move);
    SetSprite(SpriteLoader::LoadSprite("rocket").sprite);
    mslIntp->setVal("dir", Direction());
    
    data.frame_ = 0;
    mslIntp->Init({move});
}

void ProjectileEntity::Tick() {
    mslIntp->PreTick(++data.frame_);
    mslIntp->CallFunction("Tick");
    ApplyVelocity();
}

void ProjectileEntity::HandleCollision(const Entity &e1, sf::Vector2f pv) {
    
}

bool ProjectileEntity::HandleHit(const Entity *e, int f, const HitboxData &hd) {
    return false;
}
