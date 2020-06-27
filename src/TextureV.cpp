//
//  TextureV.cpp
//  main
//
//  Created by Jack Erb on 6/14/20.
//

#include "TextureV.hpp"

#include "Game/Loaders/ResourcePath.hpp"

#include <string>
#include <iostream>

#if defined(__APPLE__)
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2_image/SDL_image.h>
#else
#include "SDL.h"
#include "SDL_render.h"
#include "SDL_surface.h"
#include "SDL_image.h"
#endif

using std::cerr;
using std::endl;

TextureV::TextureV(SDL_Renderer *rd, std::string path, float scale) {
    texture_ = nullptr;
    
    path = ResourcePath() + path;
    SDL_Surface *img = IMG_Load(path.c_str());
    if (img == NULL) {
        cerr << "Failed to load image " << path << endl;
        cerr << SDL_GetError() << endl;
    } else {
        //SDL_SetColorKey(img, SDL_TRUE, SDL_MapRGB(img->format, 0, 0, 0));
        
        texture_ = SDL_CreateTextureFromSurface(rd, img);
        if (texture_ == NULL) {
            cerr << "Failed to load texture " << path << endl;
            cerr << SDL_GetError() << endl;
        } else {
            width_ = img->w * scale;
            height_ = img->h * scale;
			std::cout << "Loaded image " << path << std::endl;
        }
        SDL_FreeSurface(img);
    }
}

TextureV::~TextureV() {
    if (texture_ != nullptr) {
        SDL_DestroyTexture(texture_);
    }
}

void TextureV::render(SDL_Renderer *rd, int x, int y, bool flip) {
    x = x - width_ / 2;
    y = y - height_ / 2;
    SDL_Rect renderQuad = {x, y, (int)width_, (int)height_};
    SDL_RenderCopyEx(rd, texture_, NULL, &renderQuad, NULL, NULL, flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}
