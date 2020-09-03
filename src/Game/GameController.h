#ifndef GameController_h
#define GameController_h

#include <vector>
#include "Input.h"

class Entity;

class GameController {
  public:
    GameController();
	~GameController();

	void pretick();
	void tick();
	void render();

  private:
  	PlayerInput player_input;
    std::vector<Entity*> entities;
};

#endif  /* GameController_h */
