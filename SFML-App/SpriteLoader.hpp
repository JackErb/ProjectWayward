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
#include <SFML/Graphics.hpp>

using std::string;
using std::vector;

class SpriteLoader {
public:
    static vector<sf::Sprite*> LoadAnimation(string name, int frames) {
        vector<sf::Sprite*> res;
        
        return res;
    }
};

#endif /* SpriteLoader_hpp */
