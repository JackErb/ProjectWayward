//
//  PlatformEntity.hpp
//  SFML-App
//
//  Created by Jack Erb on 4/26/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef PlatformEntity_hpp
#define PlatformEntity_hpp

#include "StageEntity.hpp"

class PlatformEntity : public StageEntity {
public:
    PlatformEntity(int id, VectorV pos) : StageEntity(id, pos) {}
    
    void Tick() override {}
    void HandleCollision(const Entity &e1, const VectorV &pv) override {}
    bool HandleHit(const Entity *e, int f, const HitboxData &hd) override { return false; }
    void RollbackTick() override { Entity::RollbackTick(); }
    
    EntityType Type() const override { return Ent_Platform; }
};

#endif /* PlatformEntity_hpp */
