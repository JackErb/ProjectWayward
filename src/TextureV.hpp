//
//  TextureV.hpp
//  main
//
//  Created by Jack Erb on 6/14/20.
//

#ifndef TextureV_hpp
#define TextureV_hpp

#include <string>

struct SDL_Texture;
struct SDL_Renderer;

class TextureV {
public:
    TextureV(SDL_Renderer *rd, std::string path, float scale = 1.f);
    ~TextureV();
    
    void render(SDL_Renderer *rd, int x, int y, bool flip);
    
    int getWidth() { return width_; }
    int getHeight() { return height_; }
    
private:
    SDL_Texture *texture_;
    int width_;
    int height_;
};

#endif /* TextureV_hpp */
