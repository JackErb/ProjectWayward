//
//  CameraController.hpp
//  SFML-App
//
//  Created by Jack Erb on 4/28/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef CameraController_hpp
#define CameraController_hpp

#include "MathHelper.hpp"

#include <vector>
#include <list>

using std::vector;

class Entity;
struct SDL_Renderer;

typedef struct VectorFloat {
	float x, y;
} VectorFloat;

class CameraController {
public:
	bool drawHitboxes = true;

public:
	CameraController(vector<Entity*>* entities, float w, float h) : entities_(entities) {
        windowOffset_ = {w / 2.f, h / 2.f};
        cameraOffset_ = {0.f, 0.f};
	}

    void Tick();

    void Render(SDL_Renderer* window);

    //void DrawShape(const PolygonV& p, const VectorV &pos, int dir,
     //              sf::Color col, SDL_Renderer* window);

    void TransformCamera(float x, float y) { cameraOffsetFinal_ = {x,y}; }
    void SetCameraOffset(float x, float y) { cameraOffset_ = {x,y}; }
    void SetScale(float s) { scale = s; }

public:
    float scale = 0.6f;

private:
	VectorFloat windowOffset_;
	VectorFloat cameraOffset_;
	VectorFloat cameraOffsetFinal_;
    const float cameraOffsetSpeed = 40;
    vector<Entity*>* entities_;
};

#endif /* CameraController_hpp */
