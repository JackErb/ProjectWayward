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
    void handleCollision(const CollisionManifold &manifold);
    void handleHit(const CollisionManifold &manifold);

    void switchState(PlayerState *new_state);

    void updateSprite();
    void removeSprite();

    PlayerState *state;
    const PlayerInput *input;
};
