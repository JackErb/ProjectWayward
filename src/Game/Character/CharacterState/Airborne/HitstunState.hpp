//
//  HitstunState.hpp
//  main
//
//  Created by Jack Erb on 6/8/20.
//

#ifndef HitstunState_hpp
#define HitstunState_hpp

#include "AirborneState.hpp"

class HitstunState: public AirborneState {
public:
    HitstunState(Character *ch, fpoat angle, fpoat kb) : AirborneState(ch) {
        init(angle, kb);
    }
    HitstunState(Character *ch, bool rb) : AirborneState(ch) {}
    
    void ProcessInput(const PlayerInput& input) override;
    void Tick() override;
    void HandleCollision(const Entity &e, const VectorV pv) override;
    
    void SwitchState(CharState state) override;
    CharStateType GetStateType() const override {
        return Hitstun;
    }
    
private:
    void init(fpoat angle, fpoat kb);
    
};

#endif /* HitstunState_hpp */
