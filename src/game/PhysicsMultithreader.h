#ifndef PhysicsMultithreader_h
#define PhysicsMultithreader_h

#include <map>
#include <vector>
#include <ww_math.h>
#include "PhysicsTypes.h"

class Entity;
class PhysicsController;
class ChunkController;

namespace PhysicsMultithreader {

void init();

void run_partitioning(PhysicsController *pc, ChunkController *cc);
std::map<Entity*, std::vector<CollisionManifold>> run_collision_checks(PhysicsController *pc, ChunkController *cc);

void shutdown();

};

#endif  /* PhysicsMultithreader_h */
