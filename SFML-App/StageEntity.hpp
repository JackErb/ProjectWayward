//
//  StageEntity.hpp
//  SFML-App
//
//  Created by Jack Erb on 4/26/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef StageEntity_hpp
#define StageEntity_hpp

#include "Entity.hpp"

class StageEntity : public Entity {
public:
    StageEntity(int id, sf::Vector2f pos) : Entity(id, pos) {}
    
    void Tick() override {}
    void HandleCollision(const Entity &e1, sf::Vector2f pv) override {}
    void RollbackTick() override { Entity::RollbackTick(); }
    
    EntityType Type() const override { return STAGE; }
};

#endif /* StageEntity_hpp */
