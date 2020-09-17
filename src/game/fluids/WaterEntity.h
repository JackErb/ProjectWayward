#ifndef WaterEntity_h
#define WaterEntity_h

#include "Entity.h"

class WaterEntity: public Entity {
  public:
    WaterEntity(int x, int y, int r);
    ~WaterEntity();
    
    void tick();
    void handleCollision(const CollisionManifold &manifold);

    void updateSprite();
    void removeSprite();

    bool apply_gravity = true;

    int collision_count = 0;
    float r;
};

#endif  /* WaterEntity_h */
