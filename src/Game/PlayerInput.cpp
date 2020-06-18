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

const ButtonV PlayerInput::buttonNums[] = {ATTACK, SPECIAL, JUMP, SHIELD};

std::vector<int> sdlButton(ButtonV b) {
    switch (b) {
        case ATTACK: {
            std::vector<int> v;
            v.push_back(SDL_CONTROLLER_BUTTON_B);
            return v;
        }
        case SPECIAL: {
            std::vector<int> v;
            v.push_back(SDL_CONTROLLER_BUTTON_A);
            return v;
        }
        case JUMP: {
            std::vector<int> v;
            v.push_back(SDL_CONTROLLER_BUTTON_X);
            v.push_back(SDL_CONTROLLER_BUTTON_Y);
            return v;
        }
        case SHIELD: {
            std::vector<int> v;
            v.push_back(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
            v.push_back(SDL_CONTROLLER_BUTTON_RIGHTSTICK);
            return v;
        }
        case START: {
            std::vector<int> v;
            v.push_back(SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
            return v;
        }
    }
}

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
            std::vector<int> sdl_buttons = sdlButton(b);
            
            bool press = false;
            for (int sdl_b : sdl_buttons) {
                SDL_GameControllerButton sdl_b_ = (SDL_GameControllerButton)sdl_b;
                press = SDL_GameControllerGetButton(gc, sdl_b_) == 1;
                if (press) break;
            }
            bool contains = buttons.find(b) != buttons.end();
            if (press && !contains) {
                buttons[b] = PlayerInput::Pressed;
            } else if (!press && contains) {
                buttons[b] = PlayerInput::Released;
            }
        }
    } else {
        cout << "Controller not connected" << endl;
    }
}
