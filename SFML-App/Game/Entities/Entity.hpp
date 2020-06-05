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
#include <limits>
#include <string>
#include <set>
#include <unordered_map>

typedef std::vector<sf::Vector2f> Polygon;

/* Represents a bounding box of an entity w/ top left corner (x,y) */
struct Rectangle {
    float x, y, w, h;
};

typedef enum EntityType {
    CHARACTER, STAGE, PLATFORM, DECORATION
} EntityType;

class PhysicsEngine;

class Entity {
public:
    Entity(int id_, sf::Vector2f position) : id(id_), engine(nullptr) {
        data.position_ = position;
    }
    virtual ~Entity() {}
    
    /* Game Processing Functions */
    virtual void HandleCollision(const Entity &entity, sf::Vector2f pv) = 0;
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
        
        if (polygons.size() == 0) return {px, py, 0, 0};
        
        float min_x = std::numeric_limits<float>::max(),
              max_x = std::numeric_limits<float>::min(),
              min_y = min_x,
              max_y = max_x;
        for (const Polygon &p : polygons) {
            if (p.size() == 2) {
                // p is a circle
                float r = p[1].x + 0.1f;
                sf::Vector2f corner = {p[0].x - r,
                                       p[0].y - r};
                min_x = fmin(min_x, fmin(corner.x, corner.x + r * 2));
                max_x = fmax(max_x, fmax(corner.x, corner.x + r * 2));
                min_y = fmin(min_y, fmin(corner.y, corner.y + r * 2));
                max_y = fmax(max_y, fmax(corner.y, corner.y + r * 2));
            } else {
                for (const sf::Vector2f &v : p) {
                    min_x = fmin(min_x, v.x);
                    max_x = fmax(max_x, v.x);
                    min_y = fmin(min_y, v.y);
                    max_y = fmax(max_y, v.y);
                }
            }
        }
        
        return {px + min_x + .5f, py + min_y - .5f, max_x - min_x + .5f, max_y - min_y + .5f};
    }
    
    void Transform(sf::Vector2f v) {
        SetPosition(data.position_ + v);
    }
    
    void SetPosition(sf::Vector2f pos) {
        data.position_ = pos;
    }
    
    sf::Vector2f Position() const { return data.position_; }
    
    void SetSprite(sf::Sprite *s) {
        data.sprite_ = s;
    }
    
    sf::Sprite* Sprite() const { return data.sprite_; }
        
    virtual int Direction() const { return 1; }
    
    void CreateHitbox(std::string move, HitboxData data) {
        hitboxes[move][data.id] = data;
    }
    
    void SpawnHitbox(int id) {
        HitboxData d = hitboxes[move][id];
        activeHitboxes.insert(d);
    }
    
    void RemoveHitbox(int id) {
        for (auto it = activeHitboxes.begin(); it != activeHitboxes.end(); it++) {
            if (it->id == id) {
                activeHitboxes.erase(it);
                return;
            }
        }
    }
    
    void ClearHitboxes() {
        activeHitboxes.clear();
    }
        
public:
    int id;
    
    // If this object is static, it can be collided with but otherwise is not
    // affected by physics (gravity, etc.)
    bool isStatic = false;
    
    PhysicsEngine *engine;
    
    // Each polygon is a vector of (x,y) pairs describing the vertices in the
    // counterclockwise direction of this shape, where (0,0) is the entity's center
    std::vector<Polygon> polygons; // i.e. hurtboxes
    std::unordered_map<std::string, std::unordered_map<int, HitboxData>> hitboxes; // i.e. hitboxes
    
    std::string move;
    std::set<HitboxData> activeHitboxes;
    
private:
    struct GameData {
        // The top left corner of this player's bounding box
        sf::Vector2f position_;
        sf::Sprite *sprite_;
    };
    
    GameData data;
    GameData rollback_;
};

#endif /* Entity_hpp */
