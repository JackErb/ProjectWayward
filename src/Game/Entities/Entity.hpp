//
//  Entity.hpp
//  SFML-App
//
//  Created by Jack Erb on 4/25/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef Entity_hpp
#define Entity_hpp

#include "HitboxData.hpp"
#include "../../MathHelper/MathHelper.h"

#include <vector>
#include <math.h>
#include <iostream>
#include <limits>
#include <string>
#include <list>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>

typedef std::vector<VectorV> PolygonV;
class MslInterpreter;
class TextureV;

/* Represents a bounding box of an entity w/ top left corner (x,y) */
typedef struct Rectangle {
	Rectangle(fpoat x, fpoat y, fpoat w, fpoat h) : x(x), y(y), w(w), h(h) {}
	fpoat x, y, w, h;
} RectangleV;

typedef enum EntityType {
	CHARACTER, STAGE, PLATFORM, DECORATION, PROJECTILE
} EntityType;

class PhysicsEngine;

class Entity {
public:
    Entity(int id, const VectorV& pos);
    virtual ~Entity();

	/* Game Processing Functions */
	virtual void HandleCollision(const Entity& entity, const VectorV& pv) = 0;
	virtual bool HandleHit(const Entity* e, int f, const HitboxData& hd) = 0;
	virtual void Tick() = 0;

	virtual void RollbackTick() {
		rollback = data;
	}

	virtual void Rollback() {
		data = rollback;
	}

	/* Getters, Setters, & Mutators */
	virtual EntityType Type() const = 0;

    RectangleV BoundingBox() const;

    void Transform(const VectorV& v);

    void SetPosition(const VectorV& pos);
    VectorV Position() const;
    
    void SetVelocity(const fpoat& x, const fpoat& y);
    void SetVelocity(const VectorV& v);
    VectorV Velocity();
    void NullVelocityX();
    void NullVelocityY();
    
    void ApplyVelocity();

    void SetTexture(TextureV* t);
    TextureV* Texture() const;

    int Direction() const;
    void SetDirection(int d);
    
    void Freeze(int f);
    
    void SetMoveScript(const std::string &move);
    
    const std::map<int, std::list<HitboxData> >& Hitboxes() const;
    
    bool IgnoreHit(int id, int pid);
    void AddIgnoreHit(int id, int pid);

    void CreateHitbox(const std::string &move, HitboxData data);
    void SpawnHitbox(int id);
    void SpawnHitbox(int id, int pid);
    void RemoveHitbox(int id);
    void RemoveHitbox(int id, int pid);
    void ClearHitboxes();
    
private:
    void initMslBindings();

public:
	int id;
    
    bool drawPolygons = false;

	// If this object is static, it can be collided with but otherwise is not
	// affected by physics (gravity, etc.)
	bool isStatic = false;

	PhysicsEngine* engine;

	// Each polygon is a vector of (x,y) pairs describing the vertices in the
	// counterclockwise direction of this shape, where (0,0) is the entity's center
	std::vector<PolygonV> polygons; // i.e. hurtboxes
	std::unordered_map<std::string, std::unordered_map<int, std::vector<HitboxData> > > hitboxes;
    // i.e. hitboxes
    
    MslInterpreter *mslIntp;
    
protected:
    struct GameData {
        VectorV position = VectorV(fpoat(0), fpoat(0));
        VectorV velocity = VectorV(fpoat(0), fpoat(0));
        
        TextureV* texture;
        
        std::string move;
        std::map<int, std::list<HitboxData> > activeHitboxes;
        std::unordered_map<int, std::unordered_set<int> > ignoreHits;
        
        int dir = 1;
        int frame = 0;
        
        bool freeze = false;
        int freezeFr = 0;
    };
    GameData data;

private:
	GameData rollback;
};

#endif /* Entity_hpp */
