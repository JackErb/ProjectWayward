//
//  CameraController.hpp
//  SFML-App
//
//  Created by Jack Erb on 4/28/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef CameraController_hpp
#define CameraController_hpp

#include "../MathHelper/MathHelper.h"

#include <vector>
#include <list>

using std::vector;

class Entity;
struct SDL_Renderer;
struct LevelData;

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

    void DrawShape(const PolygonV& p, const VectorFloat& pos, int dir,
                   SDL_Renderer* window);

    void TransformCamera(float x, float y) { cameraOffsetFinal_ = {x,y}; }
    void SetCameraOffset(float x, float y) { cameraOffset_ = {x,y}; }
    void SetScale(float s) { scale = s; }

public:
    float scale = 0.4f;
    LevelData *level;

private:
	VectorFloat windowOffset_;
	VectorFloat cameraOffset_;
	VectorFloat cameraOffsetFinal_;
    const float cameraOffsetSpeed = 30;
    vector<Entity*>* entities_;
};

#endif /* CameraController_hpp */
