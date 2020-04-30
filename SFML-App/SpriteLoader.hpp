//
//  SpriteLoader.hpp
//  SFML-App
//
//  Created by Jack Erb on 4/28/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef SpriteLoader_hpp
#define SpriteLoader_hpp

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <SFML/Graphics.hpp>

using std::string;
using std::vector;
using std::unordered_map;

typedef unordered_map<string, vector<sf::Sprite*>> AnimMap;

class SpriteLoader {
public:
    typedef struct AnimationResult {
        vector<sf::Texture*> textures_;
        AnimMap anims;
    } AnimationResult;
    
    static AnimationResult LoadAnimations(vector<string> names);
};

#endif /* SpriteLoader_hpp */
