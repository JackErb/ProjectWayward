#ifndef PhysicsTypes_h
#define PhysicsTypes_h

#include <ww_math.h>
#include <vector>

class Entity;

typedef enum EntityDirection {
    Dir_Left, Dir_Right
} EntityDirection;

struct PolyData {
    int type;
};

/* A wrapper around collision polygons for storage of additional data & caching */
struct CollisionBox {
    // Polygons for this collision box
    std::vector<Polygon> polys;

    // bounds and poly_types vectors are 1-to-1 with polys.
    // They store additional information for the corresponding polygon.
    std::vector<PolyData> poly_data;
    std::vector<Circle> bounds;

    // Direction of the entity
    EntityDirection dir;
};

struct CollisionManifold {
    Entity *e1;
    Entity *e2;
    PolyData e1_data;
    PolyData e2_data;

    Vector2D pv;
    enum CollisionType {
        Hurtbox, Hitbox
    } type;
    int mask;
};

#endif  /* PhysicsType_h */
