//
//  CameraController.cpp
//  main
//
//  Created by Jack Erb on 6/15/20.
//

#include "CameraController.hpp"

#include "Entities/Entity.hpp"
#include "../TextureV.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

void CameraController::Tick() {
    if (cameraOffsetFinal_.x != cameraOffset_.x) {
        float a = atan2(-cameraOffset_.y + cameraOffsetFinal_.y,
            cameraOffsetFinal_.x - cameraOffset_.x);
        cameraOffset_.x += cos(a) * cameraOffsetSpeed;
        cameraOffset_.y += sin(a) * cameraOffsetSpeed;

        if (abs(cameraOffsetFinal_.x - cameraOffset_.x) < cameraOffsetSpeed + 1.f) {
            cameraOffset_.x = cameraOffsetFinal_.x;
            cameraOffset_.y = cameraOffsetFinal_.y;
        }
    }

    TransformCamera(0, 0);

    for (Entity* e : *entities_) {
        // Calculate the sprite's absolute position in the window
        VectorV pos = e->Position();
        pos = (pos + cameraOffset_) * scale + windowOffset_;

        if (e->Type() == CHARACTER) {
            if (e->Position().x > windowOffset_.x * 0.75f) {
                TransformCamera(-500, 0);
            }
            else if (e->Position().x < windowOffset_.x * -0.75f) {
                TransformCamera(500, 0);
            }
        }
    }
}


void CameraController::Render(SDL_Renderer* rd) {
    SDL_RenderSetScale(rd, scale, scale);
    for (Entity* e : *entities_) {
        VectorV pos = ((e->Position() + cameraOffset_) * scale + windowOffset_) / scale;
        bool flip = e->Direction() == -1;
        e->Texture()->render(rd, pos.x, pos.y, flip);

        if (drawHitboxes && (e->Type() == CHARACTER || e->Type() == PROJECTILE)) {
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
}

/*void CameraController::DrawShape(const PolygonV& p, const VectorV &pos, int dir,
               sf::Color col, SDL_Renderer* rd) {
    if (p.size() == 2) {
        sf::CircleShape c(p[1].x * scale);
        VectorV ps = VectorV(dir * p[0].x, p[0].y) + pos;
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
    for (const VectorV& vert : p) {
        VectorV vp = VectorV(dir * vert.x, vert.y) + pos;
        shape.setPoint(i, (vp + cameraOffset_) * scale + windowOffset_);
        i++;
    }
    shape.setFillColor(col);
    window->draw(shape);
}*/
