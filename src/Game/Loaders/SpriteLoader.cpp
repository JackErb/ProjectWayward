//
//  SpriteLoader.cpp
//  SFML-App
//
//  Created by Jack Erb on 4/28/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#include "SpriteLoader.hpp"

#include "ResourcePath.hpp"
#include "../../TextureV.hpp"

#if defined(__APPLE__)
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#else
#include "SDL.h"
#include "SDL_render.h"
#include "SDL_surface.h"
#endif

AnimMap SpriteLoader::anims = AnimMap();

AnimMap SpriteLoader::LoadAnimations(SDL_Renderer *rd, vector<pair<string, int> > names, float scale) {
    AnimMap res;
    for (auto p : names) {
        string name = p.first;
        int n = p.second;
        
        for (int i = 0; i <= n; i++) {
            res[name].push_back(LoadTexture(rd, name + "_" + std::to_string(i), scale));
        }
    }
    return res;
}

TextureV* SpriteLoader::LoadTexture(SDL_Renderer *rd, string name, float scale) {
    if (anims[name].size() == 1) return anims[name][0];
    
    TextureV *t = new TextureV(rd, name + ".png", scale);
    anims[name].push_back(t);
    return t;
}
