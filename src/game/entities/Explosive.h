#ifndef Explosive_h
#define Explosive_h

#include "Entity.h"

struct Vector2D;

class Explosive: public Entity {
  public:
    Explosive(const Vector2D &position);
    ~Explosive();

    void tick();
    void handleCollision(const CollisionManifold &manifold);

    void updateSprite();   
    void removeSprite();
};

#endif
