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

class Entity {
  public:
    Entity() {}
    ~Entity() {}

    // Game processing
    virtual void processInput(const PlayerInput &input) {}
    virtual void tick() = 0;
    virtual void handleCollision(Entity *e, const Vector2D &pv) {}
    virtual void handleHit(Entity *e, const Vector2D &pv) {}


    // Getters
    Vector2D position() const { return data.position; }
    std::vector<Polygon> polygons_hurt() const;
    std::vector<Polygon> polygons_hit() const;

    // Graphics processing
    unsigned int sprite_handle;
    virtual void updateSprite() = 0;
    virtual void removeSprite() = 0;

    // Possible polygon configurations for this entity
    std::vector<std::vector<Polygon>> hurtboxes;
    std::vector<std::vector<Polygon>> hitboxes;

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
