#ifndef GameController_h
#define GameController_h

#include <vector>

class Entity;

class GameController {
  public:
    GameController();
	~GameController();

	void pretick();
	void tick();
	void render();

  private:
    std::vector<Entity*> entities;
	std::vector<Entity*> static_entities;
};

#endif  /* GameController_h */
