#include "Input.h"

#if defined(__APPLE__)
#include <SDL2/SDL_gamecontroller.h>
#endif

#include <iostream>

using std::cout;
using std::cerr;
using std::endl;

const FixedPoint StickState::DEADZONE = FixedPoint::fromFloat(0.15f);


bool StickState::inDir(StickState::StickDir dir) const {
    if (hyp <= StickState::DEADZONE) return false;
    const FixedPoint PI_4   = FixedPoint::PI / FixedPoint::fromFloat(4);
    const FixedPoint PI_3_4 = PI_4 * FixedPoint::fromInt(3);

    switch (dir) {
      case Down:
        return angle >= -PI_3_4 && angle <= -PI_4;
      case Up:
        return angle <= PI_3_4 && angle >= PI_4;
      case Left:
        return angle >= PI_3_4 || angle <= -PI_3_4;
      case Right:
        return angle >= -PI_4 && angle <= PI_4;
    }
}

StickState readStickInput(SDL_GameController *gc) {
    StickState stick;
    int x = SDL_GameControllerGetAxis(gc, SDL_CONTROLLER_AXIS_LEFTX);
    int y = SDL_GameControllerGetAxis(gc, SDL_CONTROLLER_AXIS_LEFTY);
    stick.x = FixedPoint::fromFloat((float)x / 32767.f);
    stick.y = FixedPoint::fromFloat((float)y / 32767.f);
    stick.hyp = fp_sqrt(stick.x * stick.x + stick.y * stick.y);
    stick.angle = fp_atan2(stick.y, stick.x);

    return stick;
}

bool isButtonPressed(ButtonAction b, SDL_GameController *gc) {
    switch (b) {
      case Button_Jump:
        return SDL_GameControllerGetButton(gc, SDL_CONTROLLER_BUTTON_A) == 1;
      case Button_Attack:
        return SDL_GameControllerGetButton(gc, SDL_CONTROLLER_BUTTON_B) == 1;
      case Button_Other:
        return SDL_GameControllerGetButton(gc, SDL_CONTROLLER_BUTTON_X) == 1;
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
        for (ButtonAction b : Actions) {
            bool pressed = isButtonPressed(b, gc);
            bool contains = buttons.find(b) != buttons.end();
            if (pressed && !contains) {
                buttons[b] = ButtonState::Pressed;
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

bool PlayerInput::isPressed(ButtonAction action, bool orHeld) const {
    auto res = buttons.find(action);
    if (res == buttons.end()) return false;

    ButtonState state = res->second;
    bool held = orHeld ? state == ButtonState::Held : false;
    return held || state == ButtonState::Pressed;
}

bool PlayerInput::isReleased(ButtonAction action) const {
    auto res = buttons.find(action);
    if (res == buttons.end()) return false;

    ButtonState state = res->second;
    return state == ButtonState::Released;
}
