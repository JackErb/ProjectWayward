#include <ecs_defs.h>
#include <ecs.h>
#include <coordinator.h>
#include <logs.h>

// Physics
void PhysicsSystem::RegisterEntity(Entity e) {
  entities.insert(e);
}

void PhysicsSystem::Update() {
  for (Entity e: entities) {

  }
}
