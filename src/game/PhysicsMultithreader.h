#ifndef PhysicsMultithreader_h
#define PhysicsMultithreader_h

#include <vector>
#include <ww_math.h>

class Entity;
class GameController;

namespace PhysicsMultithreader {

struct JobReturn {
    Entity *e1;
    Entity *e2;

    // 0 for hurtbox, 1 for hitbox;
    short type;
    int bitmask;
    Vector2D pv;
};

void init();
void runJobs(std::vector<Entity*> entities, GameController *gc);
void shutdown();

};

#endif  /* PhysicsMultithreader_h */
