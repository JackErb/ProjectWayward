#ifndef Entity_h
#define Entity_h

#include <string>
#include <ww_math.h>
#include <vector>

struct PlayerInput;

const int ENTITY_DATA_MAX_BYTES = 100;

class Entity {
  public:
	Entity() {}
	~Entity() {}

	virtual void processInput(const PlayerInput &input) {}
	virtual void tick() = 0;
	virtual void handleCollision(Entity *e, const Vector2D &pv) {}

	Vector2D position() const { return data.position; }
	std::vector<Polygon> polygons() const { return hurtboxes[data.hurtbox_handle]; }


	// Graphics processing
	unsigned int sprite_handle;

	// Possible polygon configurations for this entity
	std::vector<std::vector<Polygon>> hurtboxes;
	
	// Game data
	struct GameData {
		Vector2D position;
		Vector2D velocity;

		unsigned int hurtbox_handle;
		unsigned int texture_handle;
	};
	GameData data;
};

#endif  /* Entity_h */
