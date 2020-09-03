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

bool isButtonPressed(ButtonAction b, SDL_GameController *gc) {
    switch (b) {
    case Jump:
        return SDL_GameControllerGetButton(gc, SDL_CONTROLLER_BUTTON_A) == 1;
    case Attack:
        return false;
    }
}

void PlayerInput::updateButtonsState() {
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
}

void PlayerInput::tick() {
    updateButtonsState();
    
    // Update input if the controller is connected, otherwise
    // attempt to connect to controller.
    if (SDL_IsGameController(gc_index)) {
        if (gc == NULL) {
            gc = SDL_GameControllerOpen(gc_index);
            cout << "Attempt to connect to controller " << gc_index << endl;
            cout << "\tConnected: " << (gc == NULL) << endl;
            if (gc == NULL) {
                cout << "Failed to initialize game controller" << endl;
                return;
            }
        }     

        // Read stick inputs
        stick = readStickInput(gc);
        
        // Read button inputs
        const ButtonAction actions[] = {Jump, Attack};
        for (ButtonAction b : actions) {
            bool pressed = isButtonPressed(b, gc);
            bool contains = buttons.find(b) != buttons.end();
            if (pressed && !contains) {
                cout << "Pressed!" << endl;
            } else if (!pressed && contains) {
                buttons[b] = ButtonState::Released;
            }
        }
    } else {
        if (frame % 100 == 0) {
            cout << "Controller not connected" << endl;
        }
    }
    frame++;
}
