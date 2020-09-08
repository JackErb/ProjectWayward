#include "Chunk.h"
#include <WaywardGL.h>
#include <ww_math.h>
#include <ww_generator.h>
#include "ShapeBuffer.h"
#include "SpriteBuffer.h"
#include "GameController.h"
#include <cstdlib>

using std::vector;
using std::cout;
using std::endl;

Chunk::Chunk(ChunkData chunk_data) {
    /*int chunk_x = chunk_data.chunk_x - 600;
    int chunk_y = chunk_data.chunk_y - 1000;
    int tile_w = chunk_data.tile_w;
    int tile_h = chunk_data.tile_h;
    for (int x = 0; x < chunk_data.tiles.size(); x++) {
        for (int y = 0; y < chunk_data.tiles[x].size(); y++) {
            if (chunk_data.tiles[x][y].type == Tile_Static) {
                float tile_x = chunk_x + tile_w * x;
                float tile_y = chunk_y + tile_h * y;
                //unsigned int handle = WaywardGL::addShape(tile_x, tile_y, tile_w, tile_h);
            }
        }
    }
    
    // TODO: Add proper hitbox
    vector<Polygon> polygons;
    polygons.push_back(poly_square(0, 0, 1, 1));
    hurtboxes.push_back(polygons);
    data.hurtbox_handle = 0;
    data.hitbox_handle = -1;
    data.bitmask = Bitmask::Stage;*/
}

Chunk::Chunk(int x, int y, int w, int h) {
    data.position = Vector2D(FixedPoint::fromInt(x), FixedPoint::fromInt(y));

    int texture = rand() % 3;
    sprite_handle = WaywardGL::tileBuffer()->addSprite(x, y, w, h, texture);
   
    vector<Polygon> polygons;
    polygons.push_back(poly_square(0,0,w,h));
    hurtboxes.push_back(polygons);
    data.hurtbox_handle = 0;
    
    data.hitbox_handle = -1;
    data.bitmask = Bitmask::Stage;
    data.hurtbox_bitmask |= Bitmask::Player;
}

void Chunk::tick() {
    data.position += data.velocity;
    Vector2D pos = data.position;
    float x = pos.x.toFloat(), y = pos.y.toFloat();
    WaywardGL::tileBuffer()->setSpritePos(sprite_handle, x, y);
}

void Chunk::handleCollision(Entity *e, const Vector2D &pv) {
    //data.velocity.y = FixedPoint::fromInt(40);
    //gc->removeEntity(this);
}

void Chunk::handleHit(Entity *e, const Vector2D &pv) {
    gc->removeEntity(this);
}

void Chunk::updateSprite() {
    Vector2D pos = data.position;
    float x = pos.x.toFloat(), y = pos.y.toFloat();
    WaywardGL::tileBuffer()->setSpritePos(sprite_handle, x, y);
}

void Chunk::removeSprite() {
    WaywardGL::tileBuffer()->removeSprite(sprite_handle);
}
