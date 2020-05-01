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
    PlatformEntity(int id, sf::Vector2f pos) : StageEntity(id, pos) {}
    
    void Tick() override {}
    void HandleCollision(const Entity &e1, sf::Vector2f pv) override {}
    void RollbackTick() override { Entity::RollbackTick(); }
    void Rollback() override { Entity::Rollback(); }

    
    EntityType Type() const override { return PLATFORM; }
};

#endif /* PlatformEntity_hpp */
