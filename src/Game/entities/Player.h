#include "Entity.h"

#include "Input.h"

class PlayerState;

class Player: public Entity {
  public:
    Player();
	~Player();

	void processInput(const PlayerInput &input);
	void tick();

	PlayerState *state;
};
