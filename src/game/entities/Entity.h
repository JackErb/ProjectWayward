#ifndef Entity_h
#define Entity_h

#include <string>
#include <ww_math.h>
#include <vector>
#include "PhysicsBitmask.h"

struct PlayerInput;
class GameController;

typedef enum EntityDirection {
    Dir_Left, Dir_Right
} EntityDirection;

struct CollisionBox {
    std::vector<Polygon> polys;
    std::vector<Circle> bounds;
    EntityDirection dir;
};

class Entity {
  public:
    Entity() {}
    ~Entity() {}

    // Game processing
    virtual void processInput(const PlayerInput &input) {}
    virtual void tick() = 0;
    virtual void handleCollision(Entity *e, const Vector2D &pv, int bitmask) {}
    virtual void handleHit(Entity *e, const Vector2D &pv, int bitmask) {}

    // Getters
    Vector2D position() const { return data.position; }
    const CollisionBox &polygons_hurt();
    const CollisionBox &polygons_hit();

    // Graphics processing
    unsigned int sprite_handle;
    virtual void updateSprite() = 0;
    virtual void removeSprite() = 0;


    void addHurtbox(const Polygon &hurtbox);
    void addHurtbox(const std::vector<Polygon> &hurtbox);
    void addHitbox(const Polygon &hitbox);
    void addHitbox(const std::vector<Polygon> &hitbox);

    // Possible polygon configurations for this entity
    std::vector<CollisionBox> hurtboxes;
    std::vector<CollisionBox> hitboxes;

    GameController *gc;

    // Game data
    struct GameData {
        Vector2D position;
        Vector2D velocity;
        EntityDirection dir = Dir_Left;
        int frame = 0;

        // Hurtboxes
        int hurtbox_bitmask = 0;
        int hurtbox_handle;
        // Hitboxes
        int hitbox_bitmask = 0;
        int hitbox_handle;
        // The physics type of this entity
        Bitmask::Bitmask bitmask = Bitmask::None;

        unsigned int texture_handle;
    };
    GameData data;
};

#endif  /* Entity_h */
