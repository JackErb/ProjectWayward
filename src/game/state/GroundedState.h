#ifndef GroundedState_h
#define GroundedState_h

#include "PlayerState.h"
#include "Attributes.h"
#include <ww_math.h>

typedef enum GroundedAction {
    Grounded_Neutral, Grounded_Dash, Grounded_Jumpsquat, Grounded_Land
} GroundedAction;

struct GroundedData {
    GroundedAction action;
    int frame;

    bool grounded;

    bool shorthop;
};

class GroundedState: public PlayerState {
  public:
    GroundedState(Player *p, GroundedAction action);

    void pretick();
    void tick();
    void handleCollision(const CollisionManifold &manifold);

    void switchState(PlayerState *new_state);
    void switchActionState(GroundedAction action);
    StateType type() { return State_Grounded; }

    PlayerAttributes attr;
    GroundedData data;
};

#endif  /* GroundedState_h */
