#ifndef PlayerState_h
#define PlayerState_h

#include <ww_math.h>

class Player;
struct CollisionManifold;

typedef enum StateType {
    State_Grounded, State_Airborne
} StateType;

class PlayerState {
  public:
    PlayerState() {}
    virtual ~PlayerState() {}

    virtual void pretick() = 0;
    virtual void tick() = 0;
    virtual void handleCollision(const CollisionManifold &manifold) = 0;

    virtual void switchState(PlayerState *new_state);
    virtual StateType type() = 0;

    Player *player;
};

#endif  /* PlayerState_h */
