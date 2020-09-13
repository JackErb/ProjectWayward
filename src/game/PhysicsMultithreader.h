#ifndef PhysicsMultithreader_h
#define PhysicsMultithreader_h

#include <map>
#include <vector>
#include <ww_math.h>
#include "PhysicsTypes.h"

class Entity;
class PhysicsController;

namespace PhysicsMultithreader {

void init();
std::map<Entity*, std::vector<CollisionManifold>> run(std::vector<Entity*> entities, PhysicsController *pc);
void shutdown();

};

#endif  /* PhysicsMultithreader_h */
