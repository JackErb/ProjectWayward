#include "Chunk.h"
#include <WaywardGL.h>
#include <ww_math.h>
#include <ww_generator.h>
#include "ShapeBuffer.h"

using std::vector;

Chunk::Chunk(ChunkData chunk_data) {
    int chunk_x = chunk_data.chunk_x - 600;
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
}

Chunk::Chunk(int x, int y, int w, int h) {
    data.position = Vector2D(FixedPoint::fromInt(x), FixedPoint::fromInt(y));
    
    sprite_handle = WaywardGL::shapeBuffer()->addShape(x, y, w, h);
   
    vector<Polygon> polygons;
    polygons.push_back(poly_square(0,0,w,h));
    hurtboxes.push_back(polygons);
    data.hurtbox_handle = 0;
}

void Chunk::tick() {
}
