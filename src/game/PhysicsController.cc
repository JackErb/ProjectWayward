#include "PhysicsController.h"
#include "GameController.h"

#include "PhysicsMultithreader.h"
#include "PhysicsEngine.h"
#include "Entity.h"

using std::vector;
using std::map;

PhysicsController::PhysicsController(GameController *gc) : gc(gc) {
    PhysicsMultithreader::init();
}

PhysicsController::~PhysicsController() {
    PhysicsMultithreader::shutdown();
}

vector<CollisionManifold> PhysicsController::runCollisionChecks(Entity *e1) {
    CollisionManifold manifold;
    vector<CollisionManifold> collisions;

    /* Hurtbox collisions */
    int hurtbox_mask = e1->data.hurtbox_bitmask;
    if (hurtbox_mask != 0 && e1->data.hurtbox_handle != -1) {
        for (Entity *e2 : gc->entities) {
            if (e1 == e2) continue;

            int mask = hurtbox_mask & e2->data.bitmask;
            if (mask) {
                bool collision = PhysicsEngine::checkCollision(e1, e2, &manifold);
                if (!collision) continue;

                manifold.mask = mask;
                collisions.push_back(manifold);
            }
        }
    }

    int hitbox_mask = e1->data.hitbox_bitmask;
    if (hitbox_mask != 0 && e1->data.hitbox_handle != -1) {
        for (Entity *e2 : gc->entities) {
            if (e1 == e2) continue;
            int mask = hitbox_mask & e2->data.bitmask;
            if (mask) {
                bool collision = PhysicsEngine::checkHitboxCollision(e1, e2, &manifold);
                if (!collision) continue;

                manifold.mask = mask;
                collisions.push_back(manifold);
            }
        }
    }
    return collisions;
}

void PhysicsController::runCollisionChecks() {
    manifolds = PhysicsMultithreader::run(gc->entities, this);
    notifyPriorityManifolds();
    notifyRemainingManifolds();
    manifolds.clear();
}

bool inDirection(FixedPoint angle, Direction dir) {
    const FixedPoint PI_4   = FixedPoint::PI / FixedPoint::fromFloat(4);
    const FixedPoint PI_3_4 = PI_4 * FixedPoint::fromInt(3);

    switch (dir) {
      case Down:
        return angle >= -PI_3_4 && angle <= -PI_4;
      case Up:
        return angle <= PI_3_4 && angle >= PI_4;
      case Left:
        return angle >= PI_3_4 || angle <= -PI_3_4;
      case Right:
        return angle >= -PI_4 && angle <= PI_4;
    }
}

void PhysicsController::notifyPriorityManifolds() {
    // First apply pvs based on direction.
    CollisionManifold manifold;
    for (auto it = manifolds.begin(); it != manifolds.end(); it++) {
        vector<CollisionManifold> collisions = it->second;
        for (CollisionManifold m : collisions) {
            Vector2D pos1 = m.e1->position();
            Vector2D pos2 = m.e2->position();
            FixedPoint angle = fp_atan2(pos1.y - pos2.y, pos1.x - pos2.x);
            const FixedPoint zero = FixedPoint::ZERO;
            
            //bool below = ret.pv.x == zero && ret.pv.y < zero && inDirection(angle, Down);
            bool above = m.pv.x == zero && m.pv.y < zero && inDirection(angle, Up);
            bool left = m.pv.y == zero && m.pv.x < zero && inDirection(angle, Left);
            bool right = m.pv.y == zero && m.pv.x > zero && inDirection(angle, Right);
            if (above || left || right) {
                // Collision pushing in horizontal direction
                if (PhysicsEngine::checkCollision(m.e1, m.e2, &manifold)) {
                    m.e1->handleCollision(manifold);
                }
            }
        }
    }
    // Consider removing the manifolds that were just done
}

void PhysicsController::notifyRemainingManifolds() {
    // Now apply the rest of the pvs (if still colliding)
    CollisionManifold manifold;
    for (auto it = manifolds.begin(); it != manifolds.end(); it++) {
        vector<CollisionManifold> collisions = it->second;
        for (CollisionManifold m : collisions) {
            switch (m.type) {
              case CollisionManifold::Hurtbox:
                if (PhysicsEngine::checkCollision(m.e1, m.e2, &manifold)) {
                    m.e1->handleCollision(manifold);
                }
                break;
              case CollisionManifold::Hitbox:
                if (PhysicsEngine::checkHitboxCollision(m.e1, m.e2, &manifold)) {
                    m.e2->handleHit(manifold);
                }
                break;
            }
        }
    }
}
