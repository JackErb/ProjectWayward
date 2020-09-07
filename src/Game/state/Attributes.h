#ifndef Attributes_h
#define Attributes_h

struct PlayerAttributes {
    const FixedPoint dashSpeed       = FixedPoint::fromFloat(140.f);
   
    const FixedPoint groundFriction  = FixedPoint::fromFloat(0.88f);
    const FixedPoint airFriction     = FixedPoint::fromFloat(0.91f);
    const FixedPoint gravity         = FixedPoint::fromFloat(10.5f);
    
    const FixedPoint shortJump  = FixedPoint::fromFloat(190.f);
    const FixedPoint fullJump   = FixedPoint::fromFloat(250.f);
    const FixedPoint doubleJump = FixedPoint::fromFloat(230.f);
       
    const FixedPoint airAccel    = FixedPoint::fromFloat(12.f);
    const FixedPoint maxAirSpeed = FixedPoint::fromFloat(120.f);
    const FixedPoint maxFall     = FixedPoint::fromFloat(-250.f);
    const FixedPoint maxFastFall = FixedPoint::fromFloat(-350.f);
};

#endif  /* Attributes_h */
