//
//  PhysicsEngine.cpp
//  SFML-App
//
//  Created by Jack Erb on 4/25/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#include "PhysicsEngine.hpp"

#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <SFML/Graphics.hpp>

using std::pair;
using std::make_pair;
using std::cout;
using std::endl;
using std::min;
using std::max;

vector<sf::Vector2f> get_orthogonals(const PolygonV &p1, int dir1, const PolygonV &p2, int dir2);

pair<bool, sf::Vector2f>
    is_separating_axis(const sf::Vector2f &axis,
                       const PolygonV &p1, const sf::Vector2f &pos1, int dir1,
                       const PolygonV &p2, const sf::Vector2f &pos2, int dir2);

bool PhysicsEngine::Intersects(const Entity &e1, const Entity &e2) {
    Rectangle b1 = e1.BoundingBox(), b2 = e2.BoundingBox();
    if (b1.x >= b2.x + b2.w || b2.x >= b1.x + b1.w) return false;
    if (b1.y >= b2.y + b2.h || b2.y >= b1.y + b1.h) return false;
    return true;
}

void PhysicsEngine::Update() {
    // Apply velocity to entity position, and update velocity
    // due to physical impulses (traction, gravity, etc.)
    for (Entity *e : entities_) {
        e->Tick();
    }
    
    // Check for collisions
    for (Character *character : characters_) {
        for (Entity *e : entities_) {
            if (character == e) continue;
            
            for (const PolygonV &p1 : character->polygons) {
                for (const PolygonV &p2 : e->polygons) {
                    auto res = checkCollision(p1, character->Position(), character->Direction(),
                                              p2, e->Position(), e->Direction());
                    if (res.first)
                        character->HandleCollision(*e, res.second);
                }
            }
            
            checkHitboxCollision(character, e);
        }
    }
    
    for (Entity *e : entities_) {
        if (e->Type() == CHARACTER) continue;
        for (Character *ch : characters_) {
            checkHitboxCollision(e, ch);
        }
    }
}

void PhysicsEngine::checkHitboxCollision(Entity *e1, Entity *e2) {
    for (auto it = e1->activeHitboxes.begin(); it != e1->activeHitboxes.end(); it++) {
        for (const HitboxData &h1 : it->second) {
            if (e1->ignoreHits.find(e2->id) != e1->ignoreHits.end()
                && e1->ignoreHits[e2->id].count(h1.id) == 1) {
                continue;
            }
            for (const PolygonV &p2 : e2->polygons) {
                auto res = checkCollision(h1.hitbox, e1->Position(), e1->Direction(),
                                          p2, e2->Position(), e2->Direction());
                if (res.first) {
                    int f = h1.dmg * 0.4;
                    bool r = e1->HandleHit(e1, f, h1);
                    if (r) {
                        e1->ignoreHits[e2->id].insert(h1.id);
                        e1->Freeze(f);
                    }
                }
            }
        }
    }
}

void PhysicsEngine::RollbackTick() {
    for (Entity *e : entities_) {
        e->RollbackTick();
    }
}

void PhysicsEngine::Rollback() {
    // TODO: Rollback internal state of PhysicsEngine
    
    
    for (Entity *e : entities_) {
        e->Rollback();
    }
}

pair<bool, sf::Vector2f>
  PhysicsEngine::checkCollision(const PolygonV &p1, const sf::Vector2f &pos1, int dir1,
                                const PolygonV &p2, const sf::Vector2f &pos2, int dir2) {
    // Separating Axis Theorem
    if (p1.size() == 2 && p2.size() == 2) {
        // They are both circles
        sf::Vector2f c1 = sf::Vector2f(dir1 * p1[0].x, p1[0].y) + pos1;
        sf::Vector2f c2 = sf::Vector2f(dir2 * p2[0].x, p2[0].y) + pos2;
        float dist = pow(c2.x - c1.x, 2) + pow(c2.y - c1.y, 2);
        float rad = p1[1].x + p2[1].x;
        // TODO: Calculate push vec
        if (dist < rad * rad) {
            return {true, {0,0}};
        }
        return {false, {0,0}};
    } else if (p2.size() == 2) {
        // Call with circle in first parameter
        return checkCollision(p2, pos2, dir2, p1, pos1, dir1);
    }
    
    // Get the perpendicular vectors for each side of both polygons
    vector<sf::Vector2f> orthogonals = get_orthogonals(p1, dir1, p2, dir2);
    
    // Check if there is a separating axis along each orthogonal
    vector<sf::Vector2f> push_vectors;
    for (const auto &vec : orthogonals) {
        auto res = is_separating_axis(unit_vec(vec), p1, pos1, dir1, p2, pos2, dir2);
        if (res.first) {
            // The polygons do not collide
            return make_pair(false, sf::Vector2f(0,0));
        } else {
            push_vectors.push_back(res.second);
        }
    }
    
    sf::Vector2f min_pv = push_vectors[0];
    for (int i = 1; i < push_vectors.size(); i++) {
        sf::Vector2f pv = push_vectors[i];
        if (dot(pv, pv) < dot(min_pv, min_pv)) min_pv = pv;
    }
    
    // Check that the push vector is pointing the right direction
    sf::Vector2f displacement = (geometric_center(p2) + pos2) -
      (geometric_center(p1) + pos1);
    if (dot(displacement, min_pv) > 0) {
        min_pv.x *= -1;
        min_pv.y *= -1;
    }
    
    return make_pair(true, min_pv);
}

/* Returns the orthogonal vectors of all sides of this polygon. */
vector<sf::Vector2f> get_orthogonals(const PolygonV &p1, int dir1, const PolygonV &p2, int dir2) {
    vector<sf::Vector2f> res;
    
    size_t n = p1.size();
    if (n == 2) {
        // p1 is a circle
        
        // The closest point in p2 to p1
        sf::Vector2f vec;
        float dist = std::numeric_limits<float>::max();
        int i = 0;
        for (const sf::Vector2f &v : p2) {
            float d = pow((dir2 * v.x)- (p1[0].x * dir1), 2) + pow(v.y - p1[0].y, 2);
            if (d < dist) {
                dist = d;
                vec = v;
            }
            i++;
        }
        
        res.push_back(sf::Vector2f(dir2 * vec.x, vec.y) - sf::Vector2f(dir1 * p1[0].x, p1[0].y));
        res.push_back({-res[0].y,res[0].x});
    } else {
        // Get the edge vectors
        for (int i = 0; i < n; i++) {
            sf::Vector2f pp1 = p1[(i+1) % n];
            pp1.x *= dir1;
            sf::Vector2f pp2 = p1[i];
            pp2.x *= dir1;
            res.push_back(pp1 - pp2);
        }
    }
    
    // Get the edge vectors
    long n2 = p2.size();
    for (int i = 0; i < n2; i++) {
        sf::Vector2f pp1 = p2[(i+1) % n];
        pp1.x *= dir2;
        sf::Vector2f pp2 = p2[i];
        pp2.x *= dir2;
        res.push_back(pp1 - pp2);
    }
    
    // Calculate their orthogonal
    for (int i = 0; i < res.size(); i++) {
        float x = res[i].x, y = res[i].y;
        res[i].x = -y;
        res[i].y = x;
    }
    
    return res;
}

/* Calculate if the given axis is a separating axis for the two polygons.
 * If it is, returns a pair (true, pv) where pv is the push vector
 * If not, returns the pair (false, pv) where pv is (0,0)
 */
pair<bool, sf::Vector2f> is_separating_axis(const sf::Vector2f &axis,
                                            const PolygonV &p1, const sf::Vector2f &pos1, int dir1,
                                            const PolygonV &p2, const sf::Vector2f &pos2, int dir2) {
    float min1 = std::numeric_limits<float>::max(),
          max1 = std::numeric_limits<float>::min(),
          min2 = min1,
          max2 = max1;
        
    if (p1.size() == 2) {
        // p1 is a circle
        float proj = dot(axis, sf::Vector2f(dir1 * p1[0].x, p1[0].y) + pos1);
        min1 = proj - p1[1].x;
        max1 = proj + p1[1].x;
    } else {
        for (const sf::Vector2f &vert : p1) {
            float proj = dot(axis, sf::Vector2f(dir1 * vert.x, vert.y) + pos1);
            min1 = min(proj, min1);
            max1 = max(proj, max1);
        }
    }

    for (const sf::Vector2f &vert : p2) {
        float proj = dot(axis, sf::Vector2f(dir2 * vert.x, vert.y) + pos2);
        min2 = min(proj, min2);
        max2 = max(proj, max2);
    }
    
    if (max1 >= min2 && max2 >= min1) {
        // Calculate push vector
        float d = min(max2 - min1, max1 - min2);
        sf::Vector2f push_vector((d/dot(axis, axis) + 1e-4f) * axis);
        return make_pair(false, push_vector);
    } else {
        return make_pair(true, sf::Vector2f(0.f,0.f));
    }
}