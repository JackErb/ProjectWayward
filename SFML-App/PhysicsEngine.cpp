//
//  PhysicsEngine.cpp
//  SFML-App
//
//  Created by Jack Erb on 4/25/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#include "PhysicsEngine.hpp"
#include "Entity.hpp"

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

vector<sf::Vector2f> get_orthogonals(const Polygon &p1, const Polygon &p2);

pair<bool, sf::Vector2f> is_separating_axis(const sf::Vector2f &axis, const Polygon &p1, const Polygon &p2);

float dot(const sf::Vector2f &v1, const sf::Vector2f &v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

sf::Vector2f geometric_center(const Polygon &p) {
    sf::Vector2f center;
    for (auto it = p.begin(); it != p.end(); it++) {
        center.x += it->x;
        center.y += it->y;
    }
    
    center.x /= p.size();
    center.y /= p.size();
    return center;
}

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
        e->RollbackTick();
        e->Tick();
    }
    
    // Check for collisions
    for (Character *character : characters_) {
        for (Entity *stage : stage_) {
            auto res = checkCollision(*character, *stage);
            if (res.first) {
                character->HandleCollision(*stage, res.second);
            }
        }
    }
}

void PhysicsEngine::Rollback() {
    // TODO: Rollback internal state of PhysicsEngine
    
    
    for (Entity *e : entities_) {
        e->Rollback();
    }
}

pair<bool, sf::Vector2f> PhysicsEngine::checkCollision(const Polygon &p1, const Polygon &p2) {
    // Separating Axis Theorem
    
    // Get the perpendicular vectors for each side of both polygons
    vector<sf::Vector2f> orthogonals = get_orthogonals(p1, p2);
    
    // Check if there is a separating axis along each orthogonal
    vector<sf::Vector2f> push_vectors;
    for (const auto &vec : orthogonals) {
        pair<bool, sf::Vector2f> res = is_separating_axis(vec, p1, p2);
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
    sf::Vector2f displacement = geometric_center(p2) - geometric_center(p1);
    if (dot(displacement, min_pv) > 0) {
        min_pv.x *= -1;
        min_pv.y *= -1;
    }
    
    return make_pair(true, min_pv);
}

/* Returns the orthogonal vectors of all sides of this polygon. */
vector<sf::Vector2f> get_orthogonals(const Polygon &p1, const Polygon &p2) {
    vector<sf::Vector2f> res;
    
    int n = p1.size();
    if (n == 2) {
        // p1 is a circle
        
        // The closest point in p2 to p1
        sf::Vector2f closest;
        float dist = std::numeric_limits<float>::max();
        if (p2.size() == 2) { closest = p1[0]; }
        else {
            for (const sf::Vector2f &v : p2) {
                float d = pow(v.x - p1[0].x, 2) + pow(v.y - p1[0].y, 2);
                if (d < dist) {
                    dist = d;
                    closest = v;
                }
            }
        }
        
        res.push_back(p1[0] - closest);
    } else {
        // Get the edge vectors
        for (int i = 0; i < n; i++) {
            res.push_back(p1[(i+1) % n] - p1[i]);
        }
    }
    
    n = p2.size();
    if (n == 2) {
        // p2 is a circle
        
        // The closest point in p1 to p2
        sf::Vector2f closest;
        float dist = std::numeric_limits<float>::max();
        if (p1.size() == 2) { closest = p2[0]; }
        else {
            for (const sf::Vector2f &v : p1) {
                float d = pow(v.x - p2[0].x, 2) + pow(v.y - p2[0].y, 2);
                if (d < dist) {
                    dist = d;
                    closest = v;
                }
            }
        }
        
        res.push_back(p2[0] - closest);
    } else {
        // Get the edge vectors
        for (int i = 0; i < n; i++) {
            res.push_back(p2[(i+1) % n] - p2[i]);
        }
    }
    
    // Calculate their orthogonal
    for (int i = 0; i < n; i++) {
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
                                            const Polygon &p1,
                                            const Polygon &p2) {
    float min1 = std::numeric_limits<float>::max(),
          max1 = std::numeric_limits<float>::min(),
          min2 = min1,
          max2 = max1;
        
    if (p1.size() == 2) {
        // p1 is a circle
        float proj = dot(axis, p1[0]);
        min1 = proj - p1[1].x;
        max1 = proj + p1[1].x;
    } else {
        for (const sf::Vector2f &vert : p1) {
            float proj = dot(axis, vert);
            min1 = min(proj, min1);
            max1 = max(proj, max1);
        }
    }
    
    if (p2.size() == 2) {
        // p2 is a circle
        float proj = dot(axis, p2[0]);
        min2 = proj - p2[1].x;
        max2 = proj + p2[1].x;
    } else {
        for (const sf::Vector2f &vert : p2) {
            float proj = dot(axis, vert);
            min2 = min(proj, min2);
            max2 = max(proj, max2);
        }
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
