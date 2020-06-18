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

#include <vector>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>
#include <limits>
#include <string>
#include <list>
#include <set>
#include <unordered_map>

typedef std::vector<sf::Vector2f> PolygonV;
class MslInterpreter;

/* Represents a bounding box of an entity w/ top left corner (x,y) */
typedef struct Rectangle {
	Rectangle(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {}
	float x, y, w, h;
} Rectangle;

typedef enum EntityType {
	CHARACTER, STAGE, PLATFORM, DECORATION, PROJECTILE
} EntityType;

class PhysicsEngine;

class Entity {
public:
    Entity(int id, sf::Vector2f pos);
    virtual ~Entity();

	/* Game Processing Functions */
	virtual void HandleCollision(const Entity& entity, sf::Vector2f pv) = 0;
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

	Rectangle BoundingBox() const {
		float px = data.position_.x;
		float py = data.position_.y;

		if (polygons.size() == 0) return Rectangle(px, py, 0, 0);

		float min_x = std::numeric_limits<float>::max(),
			max_x = std::numeric_limits<float>::min(),
			min_y = min_x,
			max_y = max_x;
        
        int d = Direction();
		for (const PolygonV& p : polygons) {
			if (p.size() == 2) {
				// p is a circle
				float r = p[1].x + 0.1f;
				sf::Vector2f corner = sf::Vector2f(d * p[0].x - r, p[0].y - r);
				min_x = fmin(min_x, fmin(corner.x, corner.x + r * 2));
				max_x = fmax(max_x, fmax(corner.x, corner.x + r * 2));
				min_y = fmin(min_y, fmin(corner.y, corner.y + r * 2));
				max_y = fmax(max_y, fmax(corner.y, corner.y + r * 2));
			}
			else {
				for (const sf::Vector2f& v : p) {
                    min_x = fmin(min_x, d * v.x);
					max_x = fmax(max_x, d * v.x);
					min_y = fmin(min_y, v.y);
					max_y = fmax(max_y, v.y);
				}
			}
		}
		return Rectangle(px + min_x + .5f, py + min_y - .5f, max_x - min_x + .5f, max_y - min_y + .5f);
	}

	void Transform(sf::Vector2f v) {
		SetPosition(data.position_ + v);
	}

	void SetPosition(sf::Vector2f pos) { data.position_ = pos; }
    sf::Vector2f Position() const { return data.position_; }
    
    void SetVelocity(float x, float y) { SetVelocity({x,y}); }
    void SetVelocity(sf::Vector2f v) { data.velocity_ = v; }
    sf::Vector2f Velocity() { return data.velocity_; }
    void NullVelocityX() { data.velocity_.x = 0; }
    void NullVelocityY() { data.velocity_.y = 0; }
    
    void ApplyVelocity() { Transform(data.velocity_); }

	void SetSprite(sf::Sprite* s) { data.sprite_ = s; }
	sf::Sprite* Sprite() const { return data.sprite_; }

	int Direction() const { return data.dir_; }
	void SetDirection(int d) { data.dir_ = d; }
    
    void Freeze(int f) { data.freeze_ = true; data.freezeFr_ = f; }

	void CreateHitbox(std::string move, HitboxData data) {
        hitboxes[move][data.id].push_back(data);
	}
    
    void SpawnHitbox(int id) {
        for (auto it = hitboxes[move][id].begin(); it != hitboxes[move][id].end(); it++) {
            activeHitboxes[id].push_back(*it);
        }
    }

	void SpawnHitbox(int id, int pid) {
        auto it = activeHitboxes[id].begin();
        for ( ; it != activeHitboxes[id].end(); it++) {
            if (it->pid > pid) break;
        }
        activeHitboxes[id].insert(it, hitboxes[move][id][pid]);
	}
    
    void RemoveHitbox(int id) {
        activeHitboxes[id].clear();
    }

	void RemoveHitbox(int id, int pid) {
        auto it = activeHitboxes[id].begin();
        for ( ; it != activeHitboxes[id].end(); it++) {
            if (it->pid == pid) break;
        }
        if (it == activeHitboxes[id].end()) {
            std::cerr << "Invalid RemoveHitbox(), id: " << id << std::endl;
        } else {
            activeHitboxes[id].erase(it);
            if (activeHitboxes[id].size() == 0) activeHitboxes.erase(id);
        }
	}

	void ClearHitboxes() {
		activeHitboxes.clear();
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

	std::string move;
	std::unordered_map<int, std::list<HitboxData> > activeHitboxes;
    std::unordered_map<int, std::set<int> > ignoreHits;
    
    MslInterpreter *mslIntp;
    
protected:
    struct GameData {
        // The top left corner of this player's bounding box
        sf::Vector2f position_ = sf::Vector2f(0.f, 0.f);
        sf::Vector2f velocity_ = sf::Vector2f(0.f, 0.f);
        
        sf::Sprite* sprite_;
        
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