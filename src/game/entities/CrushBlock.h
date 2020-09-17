#ifndef CrushBlock_h
#define CrushBlock_h

#include "Entity.h"

class CrushBlock: public Entity {
  public:
    CrushBlock(int x, int y, int w, int h);
    ~CrushBlock();

    void tick();
    void handleCollision(const CollisionManifold &manifold);
    void handleHit(const CollisionManifold &manifold);

    void updateSprite();
    void removeSprite();
};

#endif  /* CrushBlock_h */
