#ifndef AirborneState_h
#define AirborneState_h

#include "PlayerState.h"
#include <ww_math.h>

struct AirborneAttributes {
    const FixedPoint airAccel = FixedPoint::fromFloat(0.4f);
    const FixedPoint maxAirSpeed = FixedPoint::fromFloat(100.f);
    const FixedPoint friction = FixedPoint::fromFloat(0.88f);
    const FixedPoint gravity = FixedPoint::fromFloat(10.5f);
};

typedef enum AirborneAction {
    Airborne_Neutral
} AirborneAction;

struct AirborneData {
    AirborneAction action;
    FixedPoint accel_x;
    int frame;   
};

class AirborneState: public PlayerState {
  public:
    AirborneState(Player *p, AirborneAction action);

    void pretick();
    void tick();
    void switchState(PlayerState *new_state);
    void switchActionState(AirborneAction action);

    StateType type() { return State_Airborne; }

    AirborneAttributes attr;
    AirborneData data;
};

#endif  /* AirborneState_h */
