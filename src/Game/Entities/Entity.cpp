//
//  Entity.cpp
//  main
//
//  Created by Jack Erb on 6/12/20.
//

#include "Entity.hpp"
#include "../../MSL/MslInterpreter.hpp"
#include "../MathHelper.hpp"
#include "../Physics/PhysicsEngine.hpp"
#include "ProjectileEntity.hpp"

#include <string>

using std::string;

Entity::Entity(int id, sf::Vector2f pos) : id(id) {
    data.position_ = pos;
    
    mslIntp = new MslInterpreter();
    initMslBindings();
}

Entity::~Entity() {
    delete mslIntp;
}

void Entity::initMslBindings() {
    mslIntp->bindings_["Hitbox_Circle"] = [=]() {
        HitboxData data;
        data.id = mslIntp->getIntParam(0);
        data.pid = hitboxes[mslIntp->script][data.id].size();
        data.hitbox = {{mslIntp->getFloatParam(1), mslIntp->getFloatParam(2)}, {mslIntp->getFloatParam(3), 0}};
        CreateHitbox(mslIntp->script, data);
    };
    
    mslIntp->bindings_["Hitbox_Damage"] = [=]() {
        int id = mslIntp->getIntParam(0);
        hitboxes[mslIntp->script][id].back().dmg = mslIntp->getFloatParam(1);
    };
    
    mslIntp->bindings_["Hitbox_Knockback"] = [=]() {
        int id = mslIntp->getIntParam(0);
        float angle = mslIntp->getIntParam(1);
        string script = mslIntp->script;
        
        hitboxes[script][id].back().angle = (360 - (float)angle) * PI / 180.f;
        hitboxes[script][id].back().basekb = mslIntp->getFloatParam(2);
        hitboxes[script][id].back().kbscale = mslIntp->getFloatParam(3);
    };
    
    mslIntp->bindings_["Hitbox_Reverse"] = [=]() {
        int id = mslIntp->getIntParam(0);
        string script = mslIntp->script;
        auto back = hitboxes[script][id].back();
        hitboxes[script][id].back().reverse = !back.reverse;
    };
    
    mslIntp->bindings_["Hitbox_Spawn"] = [=]() {
        if (mslIntp->numParams() == 2) {
            SpawnHitbox(mslIntp->getIntParam(0), mslIntp->getIntParam(1));
        } else {
            SpawnHitbox(mslIntp->getIntParam(0));
        }
    };
    
    mslIntp->bindings_["Hitbox_Remove"] = [=]() {
        if (mslIntp->numParams() == 2) {
            RemoveHitbox(mslIntp->getIntParam(0), mslIntp->getIntParam(1));
        } else {
            RemoveHitbox(mslIntp->getIntParam(0));
        }
    };
    
    mslIntp->bindings_["Hitbox_Clear"] = [=]() {
        ClearHitboxes();
    };
    
    mslIntp->bindings_["Projectile_Spawn"] = [=]() {
        string script = mslIntp->getStrParam(0);
        ProjectileEntity *e = new ProjectileEntity(script, id, Position());
        e->SetDirection(Direction());
        engine->AddEntity(e);
    };
    
    mslIntp->bindings_["Delete"] = [=]() {
        engine->RemoveEntity(this);
        // TODO: Deal with cleanup
    };
    
    mslIntp->bindings_["SetVelocity"] = [=]() {
        SetVelocity(Direction() * mslIntp->getFloatParam(0), mslIntp->getFloatParam(1));
    };
}

void Entity::SetMoveScript(const std::string &move) {
    data.move = move;
    mslIntp->InitScript(move);
}
