//
//  Event.hpp
//  SFML-App
//
//  Created by Jack Erb on 4/24/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef Event_hpp
#define Event_hpp

#include <stdio.h>

class Event {
public:
    enum EventType {
        ButtonPressed, ButtonHeld, ButtonReleased,
        DirectionPressed, DirectionHeld, DirectionReleased
    };
    
    struct ButtonEvent {
        int key;
    };

    struct DirectionEvent {
        double angle;
        // Vector of joystick
    };
    
public:
    Event(EventType t) : type(t) {}
    
    enum EventType type;
    union {
        ButtonEvent buttonEvent;
        DirectionEvent directionEvent;
    };
};

#endif /* Event_hpp */
