#ifndef CrushBlock_h
#define CrushBlock_h

#include "Entity.h"

class CrushBlock: public Entity {
  public:
    CrushBlock(int x, int y, int w, int h);
    ~CrushBlock();

    void tick();
    void handleCollision(Entity *entity, const Vector2D &pv, int bitmask);

    void updateSprite();
    void removeSprite();
};

#endif  /* CrushBlock_h */
