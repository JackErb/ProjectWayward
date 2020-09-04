#ifndef Chunk_h
#define Chunk_h

#include "Entity.h"

struct ChunkData;

class Chunk: public Entity {
  public:
    Chunk(ChunkData chunk_data);
    Chunk(int x, int y, int w, int h);

    void tick();
};

#endif  /* Chunk_h */
