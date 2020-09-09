#include "Entity.h"
#include <WaywardGL.h>
#include "SpriteBuffer.h"

using std::vector;

void Entity::updateSprite() {
    float x = data.position.x.toFloat();
    float y = data.position.y.toFloat();
    WaywardGL::spriteBuffer()->setSpritePos(sprite_handle, x, y);
    WaywardGL::spriteBuffer()->setSpriteDir(sprite_handle, data.dir == Dir_Right ? 1 : 0);
}

void flipPolys(vector<Polygon> *polys) {
    for (Polygon & poly : *polys) {
        if (poly.size() == 2) {
            // Special case for circles
            poly[0].x = -poly[0].x;
            continue;
        }
        for (Vector2D &vec : poly) {
            vec.x = -vec.x;
        }
    }
}

vector<Polygon> Entity::polygons_hurt() const {
    // TODO: Somehow cache this?
    vector<Polygon> polys(hurtboxes[data.hurtbox_handle]);
    if (data.dir == Dir_Right) {
        flipPolys(&polys);
    }
    return polys;
}

vector<Polygon> Entity::polygons_hit() const {
    // TODO: Somehow cache this?
    vector<Polygon> polys(hitboxes[data.hitbox_handle]);
    if (data.dir == Dir_Right) {
        flipPolys(&polys);
    }
    return polys;
}
