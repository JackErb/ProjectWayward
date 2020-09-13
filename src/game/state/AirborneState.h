#ifndef AirborneState_h
#define AirborneState_h

#include "PlayerState.h"
#include "Attributes.h"
#include <ww_math.h>

typedef enum AirborneAction {
    Airborne_Neutral
} AirborneAction;

struct AirborneData {
    AirborneAction action;
    int jumps;
    bool fastfall;
    int frame;   
};

class AirborneState: public PlayerState {
  public:
    AirborneState(Player *p, AirborneAction action);

    void pretick();
    void tick();
    void handleCollision(const CollisionManifold &manifold);

    void switchState(PlayerState *new_state);
    void switchActionState(AirborneAction action);
    StateType type() { return State_Airborne; }

    PlayerAttributes attr;
    AirborneData data;
};

#endif  /* AirborneState_h */
