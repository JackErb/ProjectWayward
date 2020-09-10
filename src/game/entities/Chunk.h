#ifndef Chunk_h
#define Chunk_h

#include "Entity.h"

struct ChunkData;

class Chunk: public Entity {
  public:
    Chunk(int x, int y, int w, int h);

    void tick();
    void handleCollision(Entity *e, const Vector2D &pv);
    void handleHit(Entity *e, const Vector2D &pv);

    void updateSprite();
    void removeSprite();

    bool test = false;
};

#endif  /* Chunk_h */
