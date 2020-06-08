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
        StickState() : xAxis(0), yAxis(0) {}
        StickState(float x, float y) : xAxis(x), yAxis(y) {}
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
                lo = PI / 4.f; hi = PI * 3.f / 4.f;
                break;
            case RIGHT:
                lo = - PI / 4.f; hi = PI / 4.f;
                break;
            case DOWN:
                lo = - PI * 3.f / 4.f; hi = - PI / 4.f;
                break;
            case LEFT:
                return a >= PI * 3.f / 4.f || a <= - PI * 3.f / 4.f;
            case UP_T:
                lo = PI / 3.f; hi = PI * 2.f / 3.f;
                break;
            case RIGHT_T:
                lo = - PI / 6.f; hi = PI / 6.f;
                break;
            case DOWN_T:
                lo = - PI * 2.f / 3.f; hi = - PI * 1.f / 3.f;
                break;
            case LEFT_T:
                return a >= PI * 5.f / 6.f || a <= - PI * 5.f / 6.f;
        }
        return a >= lo && a <= hi;
    }
    
    constexpr const static float DEAD_ZONE = 10.f;
    
public:
    PlayerInput() : buttons() {
        stick = StickState();
        cStick = StickState();
    }

	void UpdateControllerState(unsigned int c) {
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

		stick = StickState();
		cStick = StickState();

		if (sf::Joystick::isConnected(c)) {
			// Check the controller's buttons
			for (int i = 0; i < sf::Joystick::getButtonCount(c); i++) {
				bool contains = buttons.find(i) != buttons.end();
				if (sf::Joystick::isButtonPressed(c, i) && !contains) {
					// This button was just pressed
					buttons[i] = PlayerInput::Pressed;
					// cout << "Button: " << i << std::endl;
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
		}
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
