//
//  MathHelper.h
//  SFML-App
//
//  Created by Jack Erb on 4/28/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef MathHelper_hpp
#define MathHelper_hpp

#define PI 3.1415926535
typedef std::vector<sf::Vector2f> Polygon;

float clamp(float f, float l, float h);

float dot(const sf::Vector2f &v1, const sf::Vector2f &v2);

sf::Vector2f geometric_center(const Polygon &p);

sf::Vector2f unit_vec(sf::Vector2f v);

#endif /* MathHelper_h */
