//
//  Created by Jack Erb on 6/7/20.
//  Copyright � 2020 Jack Erb. All rights reserved.
//

#include "ResourcePath.hpp"

#include <string>
#include <iostream>
#include <SDL2/SDL_filesystem.h>

std::string ResourcePath() {
    if (gResourcePath.length() == 0) {
        char *path = SDL_GetBasePath();
        gResourcePath = std::string(path);
        free(path);
        
        std::cout << SDL_GetPrefPath("Jack", "Wayward") << std::endl;
    }

    return gResourcePath;
}
