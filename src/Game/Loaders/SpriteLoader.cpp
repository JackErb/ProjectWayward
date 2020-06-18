//
//  SpriteLoader.cpp
//  SFML-App
//
//  Created by Jack Erb on 4/28/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#include "SpriteLoader.hpp"
#include "ResourcePath.hpp"

SpriteLoader::AnimationResult SpriteLoader::LoadAnimations(vector<string> names) {
    AnimMap anims;
    vector<sf::Texture*> texts;
    for (string s : names) {
        anims[s] = vector<sf::Sprite*>();
        
        sf::Texture *text = new sf::Texture();
        int i = 0;
        while (i <= 10 && text->loadFromFile(ResourcePath() + "Animations/" + s + "_" +
                                  std::to_string(i) + ".png")) {
            anims[s].push_back(new sf::Sprite(*text));
            texts.push_back(text);
            text = new sf::Texture();
            i++;
        }
        
        delete text;
    }
    
    return {texts, anims};
}

SpriteLoader::SpriteResult SpriteLoader::LoadSprite(string name) {
    sf::Texture *text = new sf::Texture();
    text->loadFromFile(ResourcePath() + name + ".png");
    return {text, new sf::Sprite(*text)};
}