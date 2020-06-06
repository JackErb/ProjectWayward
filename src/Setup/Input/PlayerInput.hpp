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
#include <math.h>

typedef enum Direction {
    /* UP: [pi/4, 3pi/4], RIGHT: [-pi/4, pi/4], etc. */
    UP, RIGHT, DOWN, LEFT,
    /* UP_T: [pi/3,2pi/3], RIGHT_T: [-pi/6, pi/6], etc. */
    UP_T, RIGHT_T, DOWN_T, LEFT_T
} Direction;

class PlayerInput {
public:
    typedef enum ButtonState {
        Pressed, Held, Released
    } ButtonState;
    
    typedef struct StickState {
        float xAxis;
        float yAxis;
        
        float hyp() const {
            return sqrt(xAxis * xAxis + yAxis * yAxis);
        }
        
        /* In range [-pi, pi] */
        float angle() const {
            return atan2(-yAxis, xAxis);
        }
        
        bool inDirection(Direction dir, float deadZone = DEAD_ZONE) const {
            if (hyp() < DEAD_ZONE) return false;
            return InDir(angle(), dir);
        }
    } StickState;
    
    static bool InRange(float a, float l, float h) {
        return a >= l && a <= h;
    }
    
    static bool InDir(float a, Direction dir) {
        float lo, hi;
        switch(dir) {
            case UP:
                lo = M_PI / 4.f; hi = M_PI * 3.f / 4.f;
                break;
            case RIGHT:
                lo = - M_PI / 4.f; hi = M_PI / 4.f;
                break;
            case DOWN:
                lo = - M_PI * 3.f / 4.f; hi = - M_PI / 4.f;
                break;
            case LEFT:
                return a >= M_PI * 3.f / 4.f || a <= - M_PI * 3.f / 4.f;
            case UP_T:
                lo = M_PI / 3.f; hi = M_PI * 2.f / 3.f;
                break;
            case RIGHT_T:
                lo = - M_PI / 6.f; hi = M_PI / 6.f;
                break;
            case DOWN_T:
                lo = - M_PI * 2.f / 3.f; hi = - M_PI * 1.f / 3.f;
                break;
            case LEFT_T:
                return a >= M_PI * 5.f / 6.f || a <= - M_PI * 5.f / 6.f;
        }
        return a >= lo && a <= hi;
    }
    
    constexpr static float DEAD_ZONE = 10.f;
    
public:
    PlayerInput() : buttons() {
        stick = {0.f, 0.f};
        cStick = {0.f, 0.f};
    }
    
    void Tick() {
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
    
    bool IsReleased(unsigned int button) const {
        auto res = buttons.find(button);
        return res != buttons.end() && res->second == Released;
    }
    
    float stickHyp() const { return stick.hyp(); }
    float stickX() const { return stick.xAxis; }
    float stickY() const { return stick.yAxis; }
    
public:
    // Contains all the buttons that are currently being pressed/held
    std::map<int, ButtonState> buttons;
    
    StickState stick;
    StickState cStick;
    
};

#endif /* PlayerInput_hpp */
