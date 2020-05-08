//
//  CharacterProperties.hpp
//  SFML-App
//
//  Created by Jack Erb on 5/6/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef CharacterProperties_hpp
#define CharacterProperties_hpp

typedef enum JumpType {
    JUP, JLEFT, JRIGHT,
    DJUMP
} JumpType;

typedef enum CharStateType {
    LandingLag, Neutral, Dash, Jumpsquat, Turnaround,
    AirborneNeutral, Airdodge
} CharacterStateType;

typedef enum CharState {
    GROUNDED, AIRBORNE
} CharState;

#endif /* CharacterProperties_hpp */
