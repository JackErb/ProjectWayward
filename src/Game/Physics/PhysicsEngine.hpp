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

#include "../Entities/Entity.hpp"
#include "../Character/Character.hpp"

struct LevelData;

using std::vector;
using std::pair;
using std::cout;
using std::endl;

/* PhysicsEngine stores and processes all the physics information of the scene.
 * It contains all the entities (plus their positions, velocities, collision boxes, etc.)
 * And calculates the physics for each time step (frame) of the game.
 */
class PhysicsEngine {
public:
    /* Check if the bounding boxes of the two entities intersect */
    static bool Intersects(const Entity &e1, const Entity &e2);
    
public:
    PhysicsEngine() {}
    void Update();
    
    void SetLevel(LevelData *level);
        
    void AddEntity(Entity *e) {
        entities_.push_back(e);
        e->engine = this;
    }
    
    void AddCharacter(Character *c) {
        characters_.push_back(c);
        AddEntity(c);
    }
    
    void AddStage(Entity *e) {
        stage_.push_back(e);
        AddEntity(e);
    }
    
    void RemoveEntity(Entity *e) {
        for (auto it = entities_.begin(); it != entities_.end(); it++) {
            if (*it == e) {
                entities_.erase(it);
                return;
            }
        }
    }
    
    bool CheckBoundingBoxAgainstPolys(const Entity *e1, const Entity *e2);
    
    void Rollback();
    void RollbackTick();
    
private:
    pair<bool, VectorV> checkCollision(const PolygonV &p1, const VectorV &pos1, int dir1,
                                            const PolygonV &p2, const VectorV &pos2, int dir2);
    void checkHitboxCollision(Entity *e1, Entity *e2);
    
private:
    vector<Entity*> entities_;
    vector<Entity*> stage_;
    vector<Character*> characters_;
    LevelData *level_;
    
    friend class GameController;
    friend class RogueController;
    int chunkId_ = 100000;
};

#endif /* PhysicsEngine_hpp */
