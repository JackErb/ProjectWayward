#ifndef PhysicsEngine_hpp
#define PhysicsEngine_hpp

#include <iostream>
#include <vector>

using std::vector;
using std::pair;
using std::cout;
using std::endl;

class Entity;
struct Vector2D;

namespace PhysicsEngine {
	bool checkCollision(const Entity *e1, const Entity* e2, Vector2D *pv);
};

#endif /* PhysicsEngine_hpp */
