#include <ecs_defs.h>
#include <physics.h>
#include <ecs.h>
#include <coordinator.h>
#include <logs.h>
#include <fp_overloads.h>
#include <vector_overloads.h>
#include <iostream>
#include <iterator>

using fp::vector;
using physics::rigidbody;

// Physics
void PhysicsSystem::RegisterEntity(Entity e) {
  Hitbox *hitbox = coordinator->GetComponent<Hitbox>(e);
  if (hitbox->type == Hitbox::entity_dynamic) {
    entities.insert(e);
  } else {
    static_entities.insert(e);
  }
}

void collision_check(Transform *t1, Hitbox *h1, Transform *t2, Hitbox *h2) {
  struct rigidbody body1 = { t1->position, &h1->polygons };
  struct rigidbody body2 = { t2->position, &h2->polygons };

  vector pv = check_collision(body1, body2);
  if (pv.x != fp::ZERO || pv.y != fp::ZERO) {
    if (pv.y > fp::ZERO) {
      t1->velocity.y = fp::ZERO;
      t1->grounded = true;
    }
    t1->position += pv;
    body1.position += pv;
  }
}

void PhysicsSystem::Update() {
  for (auto e1 = entities.begin(); e1 != entities.end(); e1++) {
    Transform *t1 = coordinator->GetComponent<Transform>(*e1);
    Hitbox *h1 = coordinator->GetComponent<Hitbox>(*e1);

    // Apply physics update
    t1->position += t1->velocity;
    t1->grounded = false;

    // @Todo: Collision detection with other dynamic entities
    /*for (auto e2 = std::next(e1); e2 != entities.end(); e2++) {
      Transform *t2 = coordinator->GetComponent<Transform>(*e2);
      Hitbox *h2 = coordinator->GetComponent<Hitbox>(*e2);
      collision_check(t1, h1, t2, h2);
    }*/

    for (auto e2 = static_entities.begin(); e2 != static_entities.end(); e2++) {
      Transform *t2 = coordinator->GetComponent<Transform>(*e2);
      Hitbox *h2 = coordinator->GetComponent<Hitbox>(*e2);
      collision_check(t1, h1, t2, h2);
    }
  }
}
