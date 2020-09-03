#ifndef PlayerInput_h
#define PlayerInput_h

#include <ww_math.h>
#include <vector>
#include<unordered_map>

enum ButtonState {
	Pressed, Released, Held
};

enum ButtonAction {
	Jump, Attack
};

typedef struct _SDL_GameController SDL_GameController;

struct PlayerInput {
	void tick();

	int gc_index;

	Vector2D stick;
	Vector2D cstick;

	std::unordered_map<ButtonAction, ButtonState> buttons;
	SDL_GameController *gc = NULL;
};

#endif  /* PlayerInput_h */
