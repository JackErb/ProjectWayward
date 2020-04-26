//
//  PlayerInput.hpp
//  SFML-App
//
//  Created by Jack Erb on 4/24/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef PlayerInput_hpp
#define PlayerInput_hpp

#include <iostream>
#include <SFML/Window.hpp>
#include <map>

class PlayerInput {
public:
    typedef enum ButtonState {
        Pressed, Held, Released
    } ButtonState;
    
    typedef struct StickState {
        float xAxis;
        float yAxis;
    } StickState;
    
public:
    PlayerInput() : buttons() {}
    
    void tick() {
        // Update all the button's state
        for (auto it = buttons.begin(); it != buttons.end(); /* No increment */) {
            if (it->second == ButtonState::Pressed) {
                it->second = ButtonState::Held;
            }
            
            // If this key has state Released, them remove it from the map
            // And update the iterator
            if (it->second == ButtonState::Released) {
                it = buttons.erase(it);
            } else {
                it++;
            }
        }
        
        stick = {0.f, 0.f};
        cStick = {0.f, 0.f};
    }
    
    bool IsPressed(unsigned int button) const {
        auto res = buttons.find(button);
        return res != buttons.end() && res->second == Pressed;
    }
    
    bool IsHeld(unsigned int button) const {
        auto res = buttons.find(button);
        return res != buttons.end() && res->second == Held;
    }

public:
    // Contains all the buttons that are currently being pressed/held
    std::map<int, ButtonState> buttons;
    
    StickState stick;
    StickState cStick;
    
};

#endif /* PlayerInput_hpp */
