//
//  CameraController.hpp
//  SFML-App
//
//  Created by Jack Erb on 4/28/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef CameraController_hpp
#define CameraController_hpp

#include <vector>
#include <SFML/Graphics.hpp>
#include "Entity.hpp"

using std::vector;

class CameraController {
public:
    bool drawHitboxes = true;
    
public:
    CameraController(vector<Entity*> *entities, float w, float h) : entities_(entities) {
        windowOffset_ = {w / 2.f, h / 2.f};
        cameraOffset_ = {0.f, 0.f};
    }
    
    void Tick() {
        if (cameraOffsetFinal_.x != cameraOffset_.x) {
            float a = atan2(- cameraOffset_.y + cameraOffsetFinal_.y,
                            cameraOffsetFinal_.x - cameraOffset_.x);
            cameraOffset_ += {cos(a) * cameraOffsetSpeed, sin(a) * cameraOffsetSpeed};
            
            if (abs(cameraOffsetFinal_.x - cameraOffset_.x) < cameraOffsetSpeed + 1.f) {
                cameraOffset_.x = cameraOffsetFinal_.x;
                cameraOffset_.y = cameraOffsetFinal_.y;
            }
        }
        
        TransformCamera(0,0);
        
        for (Entity *e : *entities_) {
            // Calculate the sprite's absolute position in the window
            sf::Vector2f pos = e->Position();
            pos += cameraOffset_;
            pos *= scale;
            pos += windowOffset_;
            
            if (e->Type() == EntityType::CHARACTER) {
                e->Sprite()->setScale(0.1515 * scale, 0.1515 * scale);
            } else {
                e->Sprite()->setScale(scale, scale);
            }
            
            sf::Rect<float> r = e->Sprite()->getGlobalBounds();
            e->Sprite()->setPosition(pos - sf::Vector2f(r.width / 2, r.height / 2));
            
            int dir = e->Direction();
            if (dir == -1) {
                float w = e->Sprite()->getGlobalBounds().width;
                e->Sprite()->scale(-1, 1);
                e->Sprite()->move(w, 0);
            }
            
            if (e->Type() == CHARACTER) {
                if (e->Position().x > windowOffset_.x * 0.75f) {
                    TransformCamera(-500, 0);
                } else if (e->Position().x < windowOffset_.x * -0.75f) {
                    TransformCamera(500, 0);
                }
            }
        }
    }
    

    void Render(sf::RenderWindow *window) {
        for (Entity *e : *entities_) {
            window->draw(*e->Sprite());
            
            if (drawHitboxes && e->Type() == CHARACTER) {
                const sf::Vector2f &pos = e->Position();
                int dir = e->Direction();
                for (const Polygon &p : e->polygons) {
                    DrawShape(p, pos, dir, sf::Color(50, 255, 50, 120), window);
                }
                
                for (const HitboxData &p : e->activeHitboxes) {
                    DrawShape(p.hitbox, pos, dir, sf::Color(255, 50, 50, 120), window);
                }
            }
            
            // TODO: Don't draw if the sprite is off the screen
        }
    }
    
    void DrawShape(const Polygon &p, const sf::Vector2f pos, int dir,
                   sf::Color col, sf::RenderWindow *window) {
        if (p.size() == 2) {
            sf::CircleShape c(p[1].x * scale);
            sf::Vector2f ps = sf::Vector2f(dir * p[0].x, p[0].y) + pos;
            ps.x -= p[1].x;
            ps.y -= p[1].x;
            c.setPosition((ps + cameraOffset_) * scale + windowOffset_);
            
            c.setFillColor(col);
            window->draw(c);
            return;
        }
        
        sf::ConvexShape shape;
        shape.setPointCount(p.size());
        int i = 0;
        for (const sf::Vector2f &vert : p) {
            sf::Vector2f vp = sf::Vector2f(dir * vert.x, vert.y) + pos;
            shape.setPoint(i, (vp + cameraOffset_) * scale + windowOffset_);
            i++;
        }
        shape.setFillColor(col);
        window->draw(shape);
    }
    
    void TransformCamera(float x, float y) {
        cameraOffsetFinal_ = {x, y};
        
    }
    void SetCameraOffset(float x, float y) { cameraOffset_ = {x, y}; }
    void SetScale(float s) { scale = s; }
    
public:
    float scale = 0.85f;
    
private:
    sf::Vector2f windowOffset_;
    sf::Vector2f cameraOffset_;
    sf::Vector2f cameraOffsetFinal_;
    const float cameraOffsetSpeed = 40;
    vector<Entity*> *entities_;
};

#endif /* CameraController_hpp */
