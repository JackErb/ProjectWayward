//
//  PlayerInput.hpp
//  SFML-App
//
//  Created by Jack Erb on 4/24/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef PlayerInput_hpp
#define PlayerInput_hpp

#include "MathHelper.hpp"

#include <iostream>
#include <map>
#include <math.h>
#include <vector>

typedef enum Direction {
    /* UP: [pi/4, 3pi/4], RIGHT: [-pi/4, pi/4], etc. */
    UP, RIGHT, DOWN, LEFT,
    /* UP_T: [pi/3,2pi/3], RIGHT_T: [-pi/6, pi/6], etc. */
    UP_T, RIGHT_T, DOWN_T, LEFT_T
} Direction;

typedef enum ButtonV {
    ATTACK, SPECIAL, JUMP, SHIELD, START
} ButtonV;

typedef struct StickDeadzones {
public:
	static fpoat DEADZONE;
	static fpoat HIGHRING;
} StickDZ;


static std::vector<int> sdlButton();

typedef struct _SDL_GameController SDL_GameController;

class PlayerInput {
public:
    typedef enum ButtonState {
        Pressed, Held, Released
    } ButtonState;
    
    typedef struct StickState {
        StickState() : x(0), y(0) {}
        StickState(fpoat x, fpoat y) : x(x), y(y) {}
        fpoat x;
		fpoat y;
        
		fpoat hyp() const {
            return fpsqrt(x * x + y * y);
        }
        
        /* In range [-pi, pi] */
		fpoat angle() const {
            return fpatan2(y, x);
        }
        
        bool inDirection(Direction dir) const {
            if (hyp() < StickDZ::DEADZONE) return false;
            return InDir(angle(), dir);
        }
    } StickState;
    
    static bool InRange(float a, float l, float h) {
        return a >= l && a <= h;
    }
    
    static bool InDir(fpoat a, Direction dir) {
        fpoat lo, hi;
		fpoat PI = FixedPoint::PI;
		fpoat PI_4TH = PI * fpoat(0, 2500);
		fpoat THREE_PI_4TH = PI * fpoat(0, 7500);
		fpoat PI_6TH = PI * fpoat(0, 1667);
        switch(dir) {
            case UP:
                lo = PI_4TH; hi = THREE_PI_4TH;
                break;
            case RIGHT:
                lo = - PI_4TH; hi = PI_4TH;
                break;
            case DOWN:
                lo = - THREE_PI_4TH; hi = - PI_4TH;
                break;
            case LEFT:
                return a >= THREE_PI_4TH || a <= - THREE_PI_4TH;
            case UP_T:
                lo = fpoat(2,0) * PI_6TH; hi = fpoat(4,0) * PI_6TH;
                break;
            case RIGHT_T:
                lo = - PI_6TH; hi = PI_6TH;
                break;
            case DOWN_T:
                lo = - fpoat(4,0) * PI_6TH; hi = - fpoat(2,0) * PI_6TH;
                break;
            case LEFT_T:
				return a >= fpoat(5, 0) * PI_6TH || a <= -fpoat(5, 0) * PI_6TH;
        }
        return a >= lo && a <= hi;
    }    
    const static ButtonV buttonNums[];
    
public:
    PlayerInput(int idx) : buttons(), idx(idx) {
        stick = StickState();
        cStick = StickState();
    }

    void UpdateControllerState();
    
    bool IsPressed(ButtonV button) const {
        auto res = buttons.find(button);
        return res != buttons.end() && res->second == Pressed;
    }
    
    bool IsHeld(ButtonV button) const {
        auto res = buttons.find(button);
        return res != buttons.end() && res->second == Held;
    }
    
    bool IsReleased(ButtonV button) const {
        auto res = buttons.find(button);
        return res != buttons.end() && res->second == Released;
    }
    
public:
    // Contains all the buttons that are currently being pressed/held
    std::map<ButtonV, ButtonState> buttons;
    
    StickState stick;
    StickState cStick;
    
    int idx;
    SDL_GameController *gc = NULL;
    
};

#endif /* PlayerInput_hpp */
