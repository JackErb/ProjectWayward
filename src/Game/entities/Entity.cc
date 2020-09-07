#include "Entity.h"
#include <WaywardGL.h>
#include "SpriteBuffer.h"

void Entity::updateSprite() {
    float x = data.position.x.toFloat();
    float y = data.position.y.toFloat();
    WaywardGL::spriteBuffer()->setSpritePos(sprite_handle, x, y);
    WaywardGL::spriteBuffer()->setSpriteDir(sprite_handle, data.dir == Dir_Right ? 1 : 0);
}

std::vector<Polygon> Entity::polygons() const {
    // TODO: Somehow cache this?
    std::vector<Polygon> polys(hurtboxes[data.hurtbox_handle]);
    if (data.dir == Dir_Right) {
        for (Polygon &poly : polys)
            for (Vector2D &vec : poly)
                vec.x = -vec.x;
    }
    return polys;
}
