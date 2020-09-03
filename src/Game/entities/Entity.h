#ifndef Entity_h
#define Entity_h

#include <string>
#include <ww_math.h>

struct PlayerInput;

class Entity {
  public:
	Entity() {}
	~Entity() {}

	virtual void processInput(const PlayerInput &input) = 0;
	virtual void tick() = 0;

	Vector2D position;
	Polygon polygons;
	
	// Graphics processing
	unsigned int sprite_handle;
	unsigned int texture_id;
};

#endif  /* Entity_h */
