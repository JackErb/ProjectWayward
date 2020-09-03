#ifndef GroundedState_h
#define GroundedState_h

#include "PlayerState.h"

class GroundedState: public PlayerState {
  public:
	GroundedState(Player *p) {
		this->player = p;
	}

	void processInput(const PlayerInput &input);
	void tick();
};

class DashState: public PlayerState {
  public:
    DashState(Player *p) {
		this->player = p;
	}

	void processInput(const PlayerInput &input);
	void tick();
};

class LandingLagState: public PlayerState {
  public:
    LandingLagState(Player *p) {
		this->player = p;
	}

	void processInput(const PlayerInput &input);
	void tick();
};

#endif  /* GroundedState_h */
