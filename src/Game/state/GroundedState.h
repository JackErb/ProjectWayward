#ifndef GroundedState_h
#define GroundedState_h

#include "PlayerState.h"
#include <ww_math.h>

struct GroundedAttributes {
    const FixedPoint dashSpeed = FixedPoint::fromFloat(100.f);
    const FixedPoint friction = FixedPoint::fromFloat(0.88f);
    const FixedPoint gravity = FixedPoint::fromFloat(10.5f);
};

typedef enum GroundedAction {
    Grounded_Neutral, Grounded_Dash, Grounded_Jumpsquat, Grounded_Land
} GroundedAction;

struct GroundedData {
    GroundedAction action;
    int frame;   
};

class GroundedState: public PlayerState {
  public:
    GroundedState(Player *p, GroundedAction action);

    void pretick();
    void tick();
    void switchState(PlayerState *new_state);
    void switchActionState(GroundedAction action);

    StateType type() { return State_Grounded; }

    GroundedAttributes attr;
    GroundedData data;
};

#endif  /* GroundedState_h */
