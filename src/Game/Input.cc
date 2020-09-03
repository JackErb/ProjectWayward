#include "Input.h"

#if defined(__APPLE__)
#include <SDL2/SDL_gamecontroller.h>
#endif

#include <iostream>

using std::cout;
using std::cerr;
using std::endl;

StickState readStickInput(SDL_GameController *gc) {
	StickState stick;
	int x = SDL_GameControllerGetAxis(gc, SDL_CONTROLLER_AXIS_LEFTX);
	int y = SDL_GameControllerGetAxis(gc, SDL_CONTROLLER_AXIS_LEFTY);
	stick.x = FixedPoint::fromFloat((double)x / 32767.0);
    stick.y = FixedPoint::fromFloat((double)y / 32767.0);
	stick.hyp = fp_sqrt(x * x + y * y);
	stick.angle = fp_atan2(y, x);

	return stick;
}

void PlayerInput::tick() {
    // Update all the button's state
    for (auto it = buttons.begin(); it != buttons.end(); /* No increment */) {
        if (it->second == ButtonState::Pressed) {
            it->second = ButtonState::Held;
        }

        // If this key has state Released, them remove it from the map
        // And update the iterator
        if (it->second == ButtonState::Released) {
            it = buttons.erase(it);
        }
        else {
            it++;
        }
    }
    
    if (SDL_IsGameController(gc_index)) {
		if (gc == NULL) {
			gc = SDL_GameControllerOpen(gc_index);
			cout << "Connected " << (gc == NULL) << endl;
			if (gc == NULL) {
				cout << "Failed to initialize game controller" << endl;
			}
			return;
		}     
  
  		stick = readStickInput(gc);

        /*for (ButtonAction b : buttonActions) {
            bool press = isActionButtonPressed(b);
            
            bool press = false;
            for (int sdl_b : sdl_buttons) {
                SDL_GameControllerButton sdl_b_ = (SDL_GameControllerButton)sdl_b;
                press = SDL_GameControllerGetButton(gc, sdl_b_) == 1;
                if (press) break;
            }
            bool contains = buttons.find(b) != buttons.end();
            if (press && !contains) {
                buttons[b] = ButtonState::Pressed;
            } else if (!press && contains) {
                buttons[b] = ButtonState::Released;
            }
        }*/
    } else {
        cout << "Controller not connected" << endl;
    }
}
