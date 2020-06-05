//
//  HitboxData.cpp
//  SFML-App
//
//  Created by Jack Erb on 6/5/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#include "HitboxData.hpp"

bool operator== (const HitboxData& p1, const HitboxData& p2) {
    return p1.id == p2.id;
}

bool operator< (const HitboxData& p1, const HitboxData& p2) {
    return p1.id < p2.id;
}
