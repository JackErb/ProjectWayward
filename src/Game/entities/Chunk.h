#ifndef Chunk_h
#define Chunk_h

#include "Entity.h"

class Chunk: public Entity {
  public:
  	Chunk();

	void tick();
};

#endif  /* Chunk_h */
