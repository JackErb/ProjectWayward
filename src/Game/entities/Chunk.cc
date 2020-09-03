#include "Chunk.h"
#include <WaywardGL.h>
#include <ww_math.h>

using std::vector;

Chunk::Chunk() {
	sprite_handle = WaywardGL::addShape(600, 600);
	//WaywardGL::updateShapePos(sprite_handle, 700, 700);
	
	vector<Polygon> polygons;
	polygons.push_back(poly_square(0, 0, 600, 600));
	hurtboxes.push_back(polygons);
	data.hurtbox_handle = 0;
}

void Chunk::tick() {
}
