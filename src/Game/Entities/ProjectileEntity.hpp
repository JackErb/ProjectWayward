//
//  ProjectileEntity.hpp
//  main
//
//  Created by Jack Erb on 6/11/20.
//

#ifndef ProjectileEntity_hpp
#define ProjectileEntity_hpp

#include "Entity.hpp"

#include <string>

class MslInterpreter;

class ProjectileEntity: public Entity {
public:
    ProjectileEntity(std::string script, int id, sf::Vector2f pos) : Entity(id, pos) {
        init(script);
    }
    
    void Tick() override;
    void HandleCollision(const Entity &e1, sf::Vector2f pv) override;
    bool HandleHit(const Entity *e, int f, const HitboxData &hd) override;
    
    void RollbackTick() override { Entity::RollbackTick(); }
    EntityType Type() const override { return PROJECTILE; }
    
private:
    void init(std::string script);
};

#endif /* ProjectileEntity_hpp */
