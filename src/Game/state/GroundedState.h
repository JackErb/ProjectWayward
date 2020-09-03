#ifndef GroundedState_h
#define GroundedState_h

#include "PlayerState.h"

class GroundedState: public PlayerState {
  public:
	GroundedState(Player *p) {
		this->player = p;
	}
	~GroundedState();

	void processInput(const PlayerInput &input);
	void tick();
};

#endif  /* GroundedState_h */
