//
//  MathHelper.cpp
//  SFML-App
//
//  Created by Jack Erb on 4/28/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

float clamp(float f, float l, float h) {
    if (f < l) return l;
    if (f > h) return h;
    return f;
}
