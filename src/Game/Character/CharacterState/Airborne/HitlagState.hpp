//
//  HitlagState.hpp
//  main
//
//  Created by Jack Erb on 6/8/20.
//

#ifndef HitlagState_hpp
#define HitlagState_hpp

#include "AirborneState.hpp"

class HitlagState: public AirborneState {
public:
    HitlagState(Character *ch, int f, float angle, float basekb, float kbscale) : AirborneState(ch) {
        init(f, angle, basekb, kbscale);
    }
    HitlagState(Character *ch, bool rb) : AirborneState(ch) {}
    
    void ProcessInput(const PlayerInput& input) override;
    void Tick() override;
    void HandleCollision(const Entity &e, const VectorV pv) override;
    
    void SwitchState(CharState state) override;
    CharStateType GetStateType() const override {
        return Hitlag;
    }
    
private:
    void init(int f, float angle, float basekb, float kbscale);
    
};

#endif /* HitlagState_hpp */
