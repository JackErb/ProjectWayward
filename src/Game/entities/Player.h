#include "Entity.h"

#include "Input.h"

class PlayerState;

class Player: public Entity {
  public:
    Player();
    ~Player();

    void processInput(const PlayerInput &input);
    void tick();
    void handleCollision(Entity *e, const Vector2D &pv);

    void switchState(PlayerState *new_state);
    void updateSprite();

    PlayerState *state;
    const PlayerInput *input;
};
