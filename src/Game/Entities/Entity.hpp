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
#include "../MathHelper.hpp"

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
		rollback_ = data;
	}

	virtual void Rollback() {
		data = rollback_;
	}

	/* Getters, Setters, & Mutators */
	virtual EntityType Type() const = 0;

	RectangleV BoundingBox() const {
		fpoat px = data.position_.x;
		fpoat py = data.position_.y;

		if (polygons.size() == 0) return RectangleV(px, py, 0, 0);

		fpoat min_x = FixedPoint::MAX;
		fpoat max_x = FixedPoint::MIN;
		fpoat min_y = min_x;
		fpoat max_y = max_x;
        
        int d = Direction();
		for (const PolygonV& p : polygons) {
			if (p.size() == 2) {
				// p is a circle
				fpoat r = p[1].x;
				VectorV corner = VectorV(d * p[0].x - r, p[0].y - r);
				min_x = fpmin(min_x, fpmin(corner.x, corner.x + r * 2));
				max_x = fpmax(max_x, fpmax(corner.x, corner.x + r * 2));
				min_y = fpmin(min_y, fpmin(corner.y, corner.y + r * 2));
				max_y = fpmax(max_y, fpmax(corner.y, corner.y + r * 2));
			}
			else {
				for (const VectorV& v : p) {
                    min_x = fpmin(min_x, d * v.x);
					max_x = fpmax(max_x, d * v.x);
					min_y = fpmin(min_y, v.y);
					max_y = fpmax(max_y, v.y);
				}
			}
		}
		return RectangleV(px + min_x + .5f, py + min_y - .5f, max_x - min_x + .5f, max_y - min_y + .5f);
	}

	void Transform(VectorV v) {
		SetPosition(data.position_ + v);
	}

	void SetPosition(VectorV pos) { data.position_ = pos; }
    VectorV Position() const { return data.position_; }
    
    void SetVelocity(fpoat x, fpoat y) { SetVelocity({x,y}); }
    void SetVelocity(VectorV v) { data.velocity_ = v; }
    VectorV Velocity() { return data.velocity_; }
    void NullVelocityX() { data.velocity_.x = 0; }
    void NullVelocityY() { data.velocity_.y = 0; }
    
    void ApplyVelocity() { Transform(data.velocity_); }

	void SetTexture(TextureV* t) { data.texture_ = t; }
	TextureV* Texture() const { return data.texture_; }

	int Direction() const { return data.dir_; }
	void SetDirection(int d) { data.dir_ = d; }
    
    void Freeze(int f) { data.freeze_ = true; data.freezeFr_ = f; }
    
    void SetMoveScript(const std::string &move);
    
    const std::map<int, std::list<HitboxData> >& Hitboxes() const { return data.activeHitboxes; }
    
    bool IgnoreHit(int id, int pid) {
        auto r = data.ignoreHits.find(id);
        if (r != data.ignoreHits.end()) {
            return data.ignoreHits[id].count(pid) != 0;
        }
        return false;
    }
    
    void AddIgnoreHit(int id, int pid) {
        data.ignoreHits[id].insert(pid);
    }

	void CreateHitbox(const std::string &move, HitboxData data) {
        hitboxes[move][data.id].push_back(data);
	}
    
    void SpawnHitbox(int id) {
        for (auto it = hitboxes[data.move][id].begin(); it != hitboxes[data.move][id].end(); it++) {
            data.activeHitboxes[id].push_back(*it);
        }
    }

	void SpawnHitbox(int id, int pid) {
        auto it = data.activeHitboxes[id].begin();
        for ( ; it != data.activeHitboxes[id].end(); it++) {
            if (it->pid > pid) break;
        }
        data.activeHitboxes[id].insert(it, hitboxes[data.move][id][pid]);
	}
    
    void RemoveHitbox(int id) {
        data.activeHitboxes[id].clear();
    }

	void RemoveHitbox(int id, int pid) {
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

	void ClearHitboxes() {
        data.activeHitboxes.clear();
	}
    
private:
    void initMslBindings();

public:
	int id;

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
        VectorV position_ = VectorV(0.f, 0.f);
        VectorV velocity_ = VectorV(0.f, 0.f);
        
        TextureV* texture_;
        
        std::string move;
        std::map<int, std::list<HitboxData> > activeHitboxes;
        std::unordered_map<int, std::unordered_set<int> > ignoreHits;
        
        int dir_ = 1;
        int frame_ = 0;
        
        bool freeze_ = false;
        int freezeFr_ = 0;
    };
    GameData data;

private:
	GameData rollback_;
};

#endif /* Entity_hpp */
