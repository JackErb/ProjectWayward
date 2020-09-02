#ifndef PhysicsEngine_hpp
#define PhysicsEngine_hpp

#include <iostream>
#include <vector>

using std::vector;
using std::pair;
using std::cout;
using std::endl;

namespace PhysicsEngine {
	bool checkCollision(const Entity *e1, const Entity* e2);
};

#endif /* PhysicsEngine_hpp */
