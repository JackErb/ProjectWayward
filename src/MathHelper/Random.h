//
//  Random.h
//  ProjectWayward
//
//  Created by Jack Erb on 7/9/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef Random_h
#define Random_h

#include <cstdint>

class Random {
public:
    Random(int64_t seed);
    
    void seed(int64_t seed);
    int64_t rand(int64_t min, int64_t max);
    float rand();
    
private:
    int64_t pv;
    double mult;
};

#endif  // Random_h
