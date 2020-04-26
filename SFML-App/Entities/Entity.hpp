//
//  Entity.hpp
//  SFML-App
//
//  Created by Jack Erb on 4/25/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef Entity_hpp
#define Entity_hpp

#include <vector>
#include <SFML/Graphics.hpp>

typedef std::vector<sf::Vector2f> Polygon;
class Entity {
public:
    Entity(int id, sf::Vector2f position) {
        this->id = id;
        this->position_ = position;
    }
    
    virtual void HandleCollision(const Entity &entity, sf::Vector2f pv) {}
    
    virtual void Tick() {}
    
    /* Getters and Setters */
    void Transform(sf::Vector2f v) {
        SetPosition(position_ + v);
    }
    
    void SetPosition(sf::Vector2f pos) {
        position_ = pos;
        sprite_.setPosition(pos);
    }
    
    sf::Vector2f Position() const { return position_; }
    
    void SetSprite(sf::Sprite s) {
        sprite_ = s;
        sprite_.setPosition(position_);
    }
    
    sf::Sprite Sprite() { return sprite_; }
        
    void SetPolygons(std::vector<Polygon> polygons) { polygons_ = polygons; }
    
    std::vector<Polygon> Polygons() const {
        std::vector<Polygon> res;
        for (int i = 0; i < polygons_.size(); i++) {
            Polygon polygon;
            for (int j = 0; j < polygons_[i].size(); j++) {
                sf::Vector2f vec(polygons_[i][j].x + position_.x,
                                 polygons_[i][j].y + position_.y);
                polygon.push_back(vec);
            }
            res.push_back(polygon);
        }
        return res;
    }
        
public:
    int id;
    sf::Vector2f velocity = {0.f, 0.f};
    
    // If this object is static, it can be collided with but otherwise is not
    // affected by physics (gravity, etc.)
    bool isStatic = false;
    
    float gravity = 0.f;
    float maxFallSpeed = 0.f;
    
private:
    // Each polygon is a vector of (x,y) pairs describing the vertices in the
    // counterclockwise direction of this shape, where (0,0) is the entity's center
    std::vector<Polygon> polygons_;
    sf::Vector2f position_;
    sf::Sprite sprite_;
};

#endif /* Entity_hpp */
