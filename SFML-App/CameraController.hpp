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
    CameraController(vector<Entity*> *entities, float w, float h) : entities_(entities) {
        windowOffset_ = {w / 2.f, h / 2.f};
        cameraOffset_ = {0.f, 0.f};
    }
    
    void Render(sf::RenderWindow *window) {
        for (Entity *e : *entities_) {
            // Calculate the sprite's absolute position in the window
            sf::Vector2f pos = e->Position();
            pos += cameraOffset_;
            pos *= scale;
            pos += windowOffset_;
            
            e->Sprite()->setPosition(pos);
            e->Sprite()->setScale(scale, scale);
            window->draw(*e->Sprite());
            
            // TODO: Don't draw if the sprite is off the screen
        }
    }
    
    void TransformCamera(float x, float y) { cameraOffset_ += {x, y}; }
    void SetCameraOffset(float x, float y) { cameraOffset_ = {x, y}; }
    void SetScale(float s) { scale = s; }
    
public:
    float scale = 0.9f;
    
private:
    sf::Vector2f windowOffset_;
    sf::Vector2f cameraOffset_;
    vector<Entity*> *entities_;
};

#endif /* CameraController_hpp */
