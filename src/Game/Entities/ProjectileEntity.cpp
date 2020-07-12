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

void ProjectileEntity::init(const std::string& move) {
    // TODO: Deal with texture
    SetMoveScript(move);
    SetTexture(SpriteLoader::anims["rocket"][0]);
    mslIntp->setVal("dir", Direction());
    
    data.frame = 0;
    mslIntp->Init({move});
}

void ProjectileEntity::Tick() {
    mslIntp->PreTick(++data.frame);
    mslIntp->CallFunction("Tick");
    ApplyVelocity();
}

void ProjectileEntity::HandleCollision(const Entity &e1, const VectorV &pv) {
    
}

bool ProjectileEntity::HandleHit(const Entity *e, int f, const HitboxData &hd) {
    return false;
}
