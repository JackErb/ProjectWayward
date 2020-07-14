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
    Jump_Up, Jump_Left, Jump_Right,
    Jump_Double
} JumpType;

typedef enum CharStateType {
    LandingLag, Neutral, Dash, Jumpsquat, Turnaround,
    AirborneNeutral, Airdodge, Hitlag, Hitstun,
    GroundedScript, AirborneScript
} CharStateType;

typedef enum CharState {
    State_Grounded, State_Airborne
} CharState;

typedef struct CharacterAttributes {
    fpoat GroundAccel = fpoat(0,2500);
    fpoat MaxGroundSpeed = fpoat(30,0);
    fpoat GroundFriction = fpoat(0,8700);
       
    fpoat AirAccel = fpoat(3,5000);
    fpoat MaxAirSpeed = fpoat(38,0);
    fpoat AirFriction = fpoat(0,9100);
       
    fpoat Gravity = fpoat(3, 4000);
    fpoat MaxFallSpeed = fpoat(40,0);
    fpoat FastFallSpeed = fpoat(70,0);
    
    fpoat DoubleJump = fpoat(65,0,true);
    fpoat WallJump = fpoat(60,0,true);
    fpoat FullhopJump = fpoat(63,0,true);
    fpoat ShorthopJump = fpoat(40,0,true);
    
    fpoat AirdodgeVelocity = fpoat(60,0);
} Attributes;

#endif /* CharacterProperties_hpp */
