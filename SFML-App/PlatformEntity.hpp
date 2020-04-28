//
//  PlatformEntity.hpp
//  SFML-App
//
//  Created by Jack Erb on 4/26/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef PlatformEntity_hpp
#define PlatformEntity_hpp

#include "StageEntity.hpp"

class PlatformEntity : public StageEntity {
public:
    PlatformEntity(int id, sf::Vector2f pos) : StageEntity(id, pos) {}
    
    void Tick() {}
    void HandleCollision(const Entity &e1, sf::Vector2f pv) {}
    EntityType Type() const { return PLATFORM; }
};

#endif /* PlatformEntity_hpp */
