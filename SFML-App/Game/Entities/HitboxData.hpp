//
//  HitboxData.hpp
//  SFML-App
//
//  Created by Jack Erb on 6/5/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef HitboxData_hpp
#define HitboxData_hpp

#include <vector>
#include <SFML/Graphics.hpp>

typedef std::vector<sf::Vector2f> Polygon;

struct HitboxData {
    int id;
    // The polygons of the hitbox, relative to to the player
    Polygon hitbox;
    
    float dmg;
    float angle; // [0, 2pi]
    float basekb;
    float kbscale;
    
    friend bool operator== (const HitboxData& p1, const HitboxData& p2);
    friend bool operator< (const HitboxData& p1, const HitboxData& p2);
};

#endif /* HitboxData_hpp */
