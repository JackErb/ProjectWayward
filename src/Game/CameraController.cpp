//
//  CameraController.cpp
//  main
//
//  Created by Jack Erb on 6/15/20.
//

#include "CameraController.hpp"

#include "../Generator/LevelData.hpp"
#include "Entities/Entity.hpp"
#include "../TextureV.hpp"

#if defined(__APPLE__)
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#else
#include "SDL.h"
#include "SDL_render.h"
#endif

#include <algorithm>

using std::min;
using std::max;

void CameraController::Tick() {
    for (Entity* e : *entities_) {
        if (e->Type() == Ent_Character) {
            TransformCamera(-e->Position().x.f(), -e->Position().y.f());
        }
    }
    
    
    float dist = (cameraOffsetFinal_.x * cameraOffsetFinal_.x)
                 + (cameraOffsetFinal_.y * cameraOffsetFinal_.y);
    
    if (dist > 80.f) {
        dist = 80.f;
    }
    
    float a = atan2(-cameraOffset_.y + cameraOffsetFinal_.y,
        cameraOffsetFinal_.x - cameraOffset_.x);
    cameraOffset_.x += cos(a) * dist / 60.f * cameraOffsetSpeed;
    cameraOffset_.y += sin(a) * dist / 60.f * cameraOffsetSpeed;

    if (sqrt(dist) < cameraOffsetSpeed) {
        cameraOffset_.x = cameraOffsetFinal_.x;
        cameraOffset_.y = cameraOffsetFinal_.y;
    }

    /*TransformCamera(0, 0);

    for (Entity* e : *entities_) {
        if (e->Type() == CHARACTER) {
            if (e->Position().x > windowOffset_.x * 0.75f) {
                TransformCamera(-500, 0);
            }
            else if (e->Position().x < windowOffset_.x * -0.75f) {
                TransformCamera(500, 0);
            }
        }
    }*/
}


void CameraController::Render(SDL_Renderer* rd) {
    SDL_RenderSetScale(rd, scale, scale);
    for (Entity* e : *entities_) {
		VectorFloat pos = { ((e->Position().x.f() + cameraOffset_.x) * scale + windowOffset_.x),
							((e->Position().y.f() + cameraOffset_.y) * scale + windowOffset_.y)};
		pos.x /= scale;
		pos.y /= scale;

        if (e->drawPolygons) {
            for (const PolygonV& poly : e->polygons) {
                DrawShape(poly, pos, e->Direction(), rd);
            }
        } else {
             e->Texture()->render(rd, pos.x, pos.y, e->Direction() == -1);
        }

        if (drawHitboxes && (e->Type() == Ent_Character || e->Type() == Ent_Projectile)) {
            const VectorV& pos = e->Position();
            int dir = e->Direction();
            for (const PolygonV& p : e->polygons) {
                //DrawShape(p, pos, dir, sf::Color(50, 255, 50, 120), rd);
            }
            
            for (auto it = e->Hitboxes().begin(); it != e->Hitboxes().end(); it++) {
                for (const HitboxData &h : it->second) {
                    //DrawShape(h.hitbox, pos, dir, sf::Color(255, 50, 50, 120), rd);
                }
            }
        }

        // TODO: Don't draw if the sprite is off the screen
    }
    
    SDL_RenderSetScale(rd, 1.f, 1.f);
}

void CameraController::DrawShape(const PolygonV& p, const VectorFloat& pos, int dir,
                                SDL_Renderer* rd) {
    SDL_SetRenderDrawColor(rd, 153, 38, 17, 255);
    int min_x = 10000000;
    int max_x = -10000000;
    int min_y = min_x, max_y = max_x;
    
    for (const auto& vert : p) {
        min_x = min(vert.x.i(), min_x);
        max_x = max(vert.x.i(), max_x);
        min_y = min(vert.y.i(), min_y);
        max_y = max(vert.y.i(), max_y);
    }
    
    SDL_Rect rect;
    rect.x = min_x + pos.x;
    rect.y = min_y + pos.y;
    rect.w = max_x - min_x;
    rect.h = max_y - min_y;
                
    SDL_RenderFillRect(rd, &rect);
}
