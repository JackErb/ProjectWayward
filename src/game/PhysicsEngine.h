#ifndef PhysicsEngine_hpp
#define PhysicsEngine_hpp

#include <iostream>
#include <vector>

using std::vector;
using std::pair;
using std::cout;
using std::endl;

class Entity;
struct CollisionManifold;

namespace PhysicsEngine {
    bool checkCollision(Entity *e1, Entity *e2, CollisionManifold *pv);
    bool checkHitboxCollision(Entity *e1, Entity *e2, CollisionManifold *pv);
};

#endif /* PhysicsEngine_hpp */
