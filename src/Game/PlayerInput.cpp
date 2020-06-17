//
//  PlayerInput.cpp
//  main
//
//  Created by Jack Erb on 6/16/20.
//

#include "PlayerInput.hpp"

#include <SDL2/SDL_gamecontroller.h>
#include <iostream>

using std::cout;
using std::endl;

const ButtonV PlayerInput::buttonNums[] = {A,B,X,Y,BACK,GUIDE,START,LSTICK,RSTICK,LB,RB,LS,RS};

void PlayerInput::UpdateControllerState() {
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
    
    if (SDL_IsGameController(idx)) {
        if (gc == NULL) gc = SDL_GameControllerOpen(idx);
        if (gc == NULL)  {
            cout << "Failed to initialize game controller" << endl;
            return;
        }
        
        
        stick.xAxis = (float)SDL_GameControllerGetAxis(gc, SDL_CONTROLLER_AXIS_LEFTX) / 32767.f;
        stick.yAxis = (float)SDL_GameControllerGetAxis(gc, SDL_CONTROLLER_AXIS_LEFTY) / 32767.f;
        
        for (ButtonV b : buttonNums) {
            bool press = SDL_GameControllerGetButton(gc, (SDL_GameControllerButton)b) == 1;
            bool contains = buttons.find(b) != buttons.end();
            if (press && !contains) {
                buttons[b] = PlayerInput::Pressed;
            } else if (!press) {
                buttons[b] = PlayerInput::Released;
            }
        }
    } else {
        cout << "Controller not connected" << endl;
    }
    
    /*if (sf::Joystick::isConnected(c)) {
        // Check the controller's buttons
        for (int i = 0; i < sf::Joystick::getButtonCount(c); i++) {
            bool contains = buttons.find(i) != buttons.end();
            if (sf::Joystick::isButtonPressed(c, i) && !contains) {
                // This button was just pressed
                buttons[i] = PlayerInput::Pressed;
                std::cout << "Button: " << i << std::endl;
            }
            else if (!sf::Joystick::isButtonPressed(c, i) && contains) {
                buttons[i] = PlayerInput::Released;
            }
        }

        // Check the controller's sticks
        stick = StickState(sf::Joystick::getAxisPosition(c, sf::Joystick::X),
                sf::Joystick::getAxisPosition(c, sf::Joystick::Y));
    } else {
        std::cerr << "Controller not connected" << std::endl;
    }*/
}
