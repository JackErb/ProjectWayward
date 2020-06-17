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

using std::string;
using std::vector;
using std::unordered_map;
using std::pair;

class TextureV;
struct SDL_Renderer;
typedef unordered_map< string, vector<TextureV*> > AnimMap;

class SpriteLoader {
public:
    static AnimMap LoadAnimations(SDL_Renderer *rd, vector<pair<string, int> > names, float scale = 1.f);
    static TextureV* LoadTexture(SDL_Renderer *rd, string name, float scale = 1.f);
    
    static AnimMap anims;
};

#endif /* SpriteLoader_hpp */
