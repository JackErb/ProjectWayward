//
//  PhysicsEngine.hpp
//  SFML-App
//
//  Created by Jack Erb on 4/25/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef PhysicsEngine_hpp
#define PhysicsEngine_hpp

#include <iostream>
#include <vector>

#include "Entity.hpp"
#include "Character.hpp"

using std::vector;
using std::pair;
using std::cout;
using std::endl;

/* PhysicsEngine stores and processes all the physics information of the scene.
 * It contains all the entities (plus their positions, velocities, collision boxes, etc.)
 * And calculates the physics for each time step (frame) of the game.
 * Will alert delegate_ (if set) of things like collisions, etc.
 */
class PhysicsEngine {
public:
    /* Check if the bounding boxes of the two entities intersect */
    static bool Intersects(const Entity &e1, const Entity &e2);
    
public:
    PhysicsEngine() {}
    void Update();
        
    void AddEntity(Entity *e) {
        entities_.push_back(e);
        e->engine = this;
    }
    
    void AddCharacter(Character *c) {
        characters_.push_back(c);
        AddEntity(c);
    }
    
    void AddStage(Entity *e) {
        assert(e->Type() == STAGE || e->Type() == PLATFORM);
        stage_.push_back(e);
        AddEntity(e);
    }
    
    void RemoveEntity(int id) {
        for (auto it = entities_.begin(); it != entities_.end(); it++) {
            if ((*it)->id == id) {
                entities_.erase(it);
                return;
            }
        }
    }
    
    bool CheckBoundingBoxCollision(const Entity *e1, const Entity *e2) {
        return Intersects(*e1, *e2);
    }
    
    void Rollback();
    void RollbackTick();
    
private:
    pair<bool, sf::Vector2f> checkCollision(const Entity &e1, const Entity &e2) {
        for (const Polygon &p1 : e1.Polygons()) {
            for (const Polygon &p2 : e2.Polygons()) {
                auto res = checkCollision(p1, p2);
                if (res.first)
                    return res;
            }
        }
        return std::make_pair(false, sf::Vector2f(0,0));
    }
    pair<bool, sf::Vector2f> checkCollision(const Polygon &p1, const Polygon &p2);
    
private:
    vector<Entity*> entities_;
    vector<Entity*> stage_;
    vector<Character*> characters_;
};

#endif /* PhysicsEngine_hpp */
