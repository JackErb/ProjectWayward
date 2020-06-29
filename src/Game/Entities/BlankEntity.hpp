//
//  BlankEntity.hpp
//  SFML-App
//
//  Created by Jack Erb on 5/1/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef BlankEntity_hpp
#define BlankEntity_hpp

#include "Entity.hpp"

class BlankEntity : public Entity {
public:
    BlankEntity(int id, sf::Vector2f pos) : Entity(id, pos) {
        isStatic = true;
    }
    
    void Tick() override {}
    void HandleCollision(const Entity &e1, const VectorV &pv) override {}
    bool HandleHit(const Entity *e, int f, const HitboxData &hd) override { return false; }
    void RollbackTick() override { Entity::RollbackTick(); }
    
    EntityType Type() const override { return DECORATION; }
};

#endif /* BlankEntity_hpp */
