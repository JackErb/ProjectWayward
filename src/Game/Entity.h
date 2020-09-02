#ifndef Entity_h
#define Entity_h

#include <string>

class Entity {
  public:
	Entity(unsigned int texture_id);
	~Entity();

	float x;
	float y;
	unsigned int texture_id;
};

#endif  /* Entity_h */
