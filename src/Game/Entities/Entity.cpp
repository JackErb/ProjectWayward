//
//  Entity.cpp
//  main
//
//  Created by Jack Erb on 6/12/20.
//

#include "Entity.hpp"
#include "../../MSL/MslInterpreter.hpp"
#include "../Physics/PhysicsEngine.hpp"
#include "ProjectileEntity.hpp"

#include <string>

using std::string;

Entity::Entity(int id, const VectorV &pos) : id(id) {
    data.position = pos;
    
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
        
        fpoat x = FixedPoint::FromFloat(mslIntp->getFloatParam(1));
        fpoat y = FixedPoint::FromFloat(mslIntp->getFloatParam(2));
        fpoat r = FixedPoint::FromFloat(mslIntp->getFloatParam(3));
        data.hitbox = {{x,y}, {r,0}};
        CreateHitbox(mslIntp->script, data);
    };
    
    mslIntp->bindings_["Hitbox_Damage"] = [=]() {
        int id = mslIntp->getIntParam(0);
        hitboxes[mslIntp->script][id].back().dmg = FixedPoint::FromFloat(mslIntp->getFloatParam(1));
    };
    
    mslIntp->bindings_["Hitbox_Knockback"] = [=]() {
        int id = mslIntp->getIntParam(0);
        int angle = mslIntp->getIntParam(1);
        string script = mslIntp->script;
        
        
        hitboxes[script][id].back().angle = FixedPoint::FromInt(360-angle) * FixedPoint::PI / fpoat(180,0);
        hitboxes[script][id].back().basekb = FixedPoint::FromFloat(mslIntp->getFloatParam(2));
        hitboxes[script][id].back().kbscale = FixedPoint::FromFloat(mslIntp->getFloatParam(3));
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
        fpoat vy = FixedPoint::FromFloat(mslIntp->getFloatParam(1));
        fpoat vx = FixedPoint::FromFloat(mslIntp->getFloatParam(0));
        if (Direction() == -1) vx.sign = !vx.sign;
        SetVelocity(vx, vy);
    };
}

void Entity::SetMoveScript(const std::string &move) {
    data.move = move;
    mslIntp->InitScript(move);
}

RectangleV Entity::BoundingBox() const {
    if (polygons.size() == 0) return RectangleV(0, 0, 0, 0);

    fpoat min_x = FixedPoint::MAX;
    fpoat max_x = FixedPoint::MIN;
    fpoat min_y = min_x;
    fpoat max_y = max_x;
    
    for (const PolygonV& p : polygons) {
        if (p.size() == 2) {
            // p is a circle
            fpoat r = p[1].x;
            VectorV corner = VectorV(p[0].x - r, p[0].y - r);
            min_x = fpmin(min_x, corner.x);
            max_x = fpmax(max_x, corner.x + r * 2);
            min_y = fpmin(min_y, corner.y);
            max_y = fpmax(max_y, corner.y + r * 2);
        }
        else {
            for (const VectorV& v : p) {
                min_x = fpmin(min_x, v.x);
                max_x = fpmax(max_x, v.x);
                min_y = fpmin(min_y, v.y);
                max_y = fpmax(max_y, v.y);
            }
        }
    }

    fpoat f = fpoat(1);
    return RectangleV(min_x - f, min_y - f, max_x - min_x + fpoat(2,0) * f,
                      max_y - min_y + fpoat(2,0) * f);
}

void Entity::Transform(const VectorV& v) {
    SetPosition(data.position + v);
}

void Entity::SetPosition(const VectorV& pos) { data.position = pos; }
VectorV Entity::Position() const { return data.position; }

void Entity::SetVelocity(const fpoat& x, const fpoat& y) { SetVelocity({x,y}); }
void Entity::SetVelocity(const VectorV& v) { data.velocity = v; }
VectorV Entity::Velocity() { return data.velocity; }
void Entity::NullVelocityX() { data.velocity.x = 0; }
void Entity::NullVelocityY() { data.velocity.y = 0; }

void Entity::ApplyVelocity() { Transform(data.velocity); }

void Entity::SetTexture(TextureV* t) { data.texture = t; }
TextureV* Entity::Texture() const { return data.texture; }

int Entity::Direction() const { return data.dir; }
void Entity::SetDirection(int d) { data.dir = d; }

void Entity::Freeze(int f) { data.freeze = true; data.freezeFr = f; }

const std::map<int, std::list<HitboxData> >& Entity::Hitboxes() const { return data.activeHitboxes; }

bool Entity::IgnoreHit(int id, int pid) {
    auto r = data.ignoreHits.find(id);
    if (r != data.ignoreHits.end()) {
        return data.ignoreHits[id].count(pid) != 0;
    }
    return false;
}

void Entity::AddIgnoreHit(int id, int pid) {
    data.ignoreHits[id].insert(pid);
}

void Entity::CreateHitbox(const std::string &move, HitboxData data) {
    hitboxes[move][data.id].push_back(data);
}

void Entity::SpawnHitbox(int id) {
    for (auto it = hitboxes[data.move][id].begin(); it != hitboxes[data.move][id].end(); it++) {
        data.activeHitboxes[id].push_back(*it);
    }
}

void Entity::SpawnHitbox(int id, int pid) {
    auto it = data.activeHitboxes[id].begin();
    for ( ; it != data.activeHitboxes[id].end(); it++) {
        if (it->pid > pid) break;
    }
    data.activeHitboxes[id].insert(it, hitboxes[data.move][id][pid]);
}

void Entity::RemoveHitbox(int id) {
    data.activeHitboxes[id].clear();
}

void Entity::RemoveHitbox(int id, int pid) {
    auto it = data.activeHitboxes[id].begin();
    for ( ; it != data.activeHitboxes[id].end(); it++) {
        if (it->pid == pid) break;
    }
    if (it == data.activeHitboxes[id].end()) {
        std::cerr << "Invalid RemoveHitbox(), id: " << id << std::endl;
    } else {
        data.activeHitboxes[id].erase(it);
        if (data.activeHitboxes[id].size() == 0) data.activeHitboxes.erase(id);
    }
}

void Entity::ClearHitboxes() {
    data.activeHitboxes.clear();
}
