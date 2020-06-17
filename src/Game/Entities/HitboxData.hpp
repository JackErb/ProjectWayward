//
//  HitboxData.hpp
//  SFML-App
//
//  Created by Jack Erb on 6/5/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef HitboxData_hpp
#define HitboxData_hpp

#include "../MathHelper.hpp"

#include <vector>

struct HitboxData {
    int id;
    int pid; // priority id
    
    // The polygons of the hitbox, relative to to the player
    PolygonV hitbox;
    
    float dmg;
    float angle; // [0, 2pi]
    float basekb;
    float kbscale;
    bool reverse = false;
    
    friend bool operator== (const HitboxData& p1, const HitboxData& p2);
    friend bool operator< (const HitboxData& p1, const HitboxData& p2);
};

#endif /* HitboxData_hpp */
