#include "Entity.h"

#include "Input.h"
#include <iostream>

class PlayerState;

class Player: public Entity {
  public:
    Player();
    ~Player();

    void processInput(const PlayerInput &input);
    void tick();
    void handleCollision(Entity *e, const Vector2D &pv);
    void handleHit(Entity *e, const Vector2D &pv);

    void switchState(PlayerState *new_state);

    void updateSprite();
    void removeSprite();

    PlayerState *state;
    const PlayerInput *input;
};
