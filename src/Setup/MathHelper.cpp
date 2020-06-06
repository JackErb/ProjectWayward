//
//  MathHelper.cpp
//  SFML-App
//
//  Created by Jack Erb on 4/28/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#include <vector>
#include <SFML/Graphics.hpp>
#include <math.h>

typedef std::vector<sf::Vector2f> Polygon;

float clamp(float f, float l, float h) {
    if (f < l) return l;
    if (f > h) return h;
    return f;
}

float dot(const sf::Vector2f &v1, const sf::Vector2f &v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

sf::Vector2f geometric_center(const Polygon &p) {
    // Special case for circles
    if (p.size() == 2) return p[0];
    
    sf::Vector2f center;
    for (auto it = p.begin(); it != p.end(); it++) {
        center.x += it->x;
        center.y += it->y;
    }
    
    center.x /= p.size();
    center.y /= p.size();
    return center;
}

sf::Vector2f unit_vec(sf::Vector2f v) {
    float hyp = sqrt((v.x * v.x) + (v.y * v.y));
    return v / hyp;
}
