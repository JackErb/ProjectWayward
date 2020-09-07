#ifndef Entity_h
#define Entity_h

#include <string>
#include <ww_math.h>
#include <vector>

struct PlayerInput;

typedef enum EntityDirection {
    Dir_Left, Dir_Right
} EntityDirection;

class Entity {
  public:
    Entity() {}
    ~Entity() {}

    virtual void processInput(const PlayerInput &input) {}
    virtual void tick() = 0;
    virtual void handleCollision(Entity *e, const Vector2D &pv) {}

    Vector2D position() const { return data.position; }
    std::vector<Polygon> polygons() const;

    virtual void updateSprite() = 0;

    // Graphics processing
    unsigned int sprite_handle;

    // Possible polygon configurations for this entity
    std::vector<std::vector<Polygon>> hurtboxes;
 

    // Game data
    struct GameData {
        Vector2D position;
        Vector2D velocity;
        EntityDirection dir = Dir_Left;

        unsigned int hurtbox_handle;
        unsigned int texture_handle;
    };
    GameData data;
};

#endif  /* Entity_h */
