#ifndef Chunk_h
#define Chunk_h

#include "Entity.h"

struct ChunkData;

class Chunk: public Entity {
  public:
    Chunk(int x, int y, int w, int h);

    void tick();
    void handleCollision(const CollisionManifold &manifold);
    void handleHit(const CollisionManifold &manifold);

    void updateSprite();
    void removeSprite();
};

#endif  /* Chunk_h */
