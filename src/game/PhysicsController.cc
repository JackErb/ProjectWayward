#include "PhysicsController.h"
#include "GameController.h"
#include "ChunkController.h"

#include "PhysicsMultithreader.h"
#include "PhysicsEngine.h"
#include "Entity.h"

using std::vector;
using std::map;
using std::set;
using std::list;

PhysicsController::PhysicsController(GameController *gc) {
    game_controller = gc;

    PhysicsMultithreader::init();
}

PhysicsController::~PhysicsController() {
    PhysicsMultithreader::shutdown();
}

void PhysicsController::setChunkController(ChunkController *cc) {
    chunk_controller = cc;
}

template <class Iter>
void runHurtboxChecks(Entity *e1, const Iter &entities, vector<CollisionManifold> &collisions) {
    CollisionManifold manifold;
    for (Entity *e2 : entities) {
        if (e1 == e2) continue;

        int mask = e1->data.hurtbox_bitmask & e2->data.bitmask;
        if (mask) {
            bool collision = PhysicsEngine::checkCollision(e1, e2, &manifold);
            if (!collision) continue;

            collisions.push_back(manifold);
        }
    }
}

template <class Iter>
void runHitboxChecks(Entity *e1, const Iter &entities, vector<CollisionManifold> &collisions) {
    CollisionManifold manifold;
    for (Entity *e2 : entities) {
        if (e1 == e2) continue;

        int mask = e1->data.bitmask && e2->data.hitbox_bitmask;
        if (mask) {
            bool collision = PhysicsEngine::checkHitboxCollision(e1, e2, &manifold);
            if (!collision) continue;

            collisions.push_back(manifold);
        }
    }
}

void runCollisionChecksOnEntity(Entity *e1, const ChunkContainer &chunk, vector<CollisionManifold> &collisions) {
    CollisionManifold manifold;

    /* Hurtbox collisions */
    if (e1->data.hurtbox_bitmask != 0 && e1->data.hurtbox_handle != -1) {
        runHurtboxChecks(e1, chunk.entities, collisions);
        runHurtboxChecks(e1, chunk.static_entities, collisions);
        runHurtboxChecks(e1, chunk.other_entities, collisions);
    }

    if (e1->data.hitbox_handle != -1) {
        runHitboxChecks(e1, chunk.entities, collisions);
        runHitboxChecks(e1, chunk.static_entities, collisions);
        runHitboxChecks(e1, chunk.other_entities, collisions);
    }
}

vector<CollisionManifold> PhysicsController::runCollisionChecks(const ChunkContainer &chunk) {
    vector<CollisionManifold> collisions;
    for (Entity *entity : chunk.entities) {
        runCollisionChecksOnEntity(entity, chunk, collisions);
    }

    for (Entity *entity : chunk.other_entities) {
        runCollisionChecksOnEntity(entity, chunk, collisions);
    }

    return collisions;
}

void PhysicsController::runCollisionChecks() {
    chunk_controller->resetFrame();

    // Partition entities into chunks. The returned vector contains entities that
    // have moved chunks and need to be updated.
    PhysicsMultithreader::run_partitioning(this, chunk_controller);

    // Run the collision checks
    auto manifolds = PhysicsMultithreader::run_collision_checks(this, chunk_controller);
    // Notify colliding entities
    notifyPriorityManifolds(manifolds);
    notifyRemainingManifolds(manifolds);
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

void PhysicsController::notifyPriorityManifolds(const map<Entity*, vector<CollisionManifold>> &manifolds) {
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

void PhysicsController::notifyRemainingManifolds(const map<Entity*, vector<CollisionManifold>> &manifolds) {
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
