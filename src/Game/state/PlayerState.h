#ifndef PlayerState_h
#define PlayerState_h

struct PlayerInput;
class Player;


class PlayerState {
  public:
	PlayerState() {}
	~PlayerState() {}

	virtual void processInput(const PlayerInput &input) = 0;
	virtual void tick() = 0;

	Player *player;
};

#endif  /* PlayerState_h */
