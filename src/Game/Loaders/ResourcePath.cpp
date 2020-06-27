//
//  Created by Jack Erb on 6/7/20.
//  Copyright � 2020 Jack Erb. All rights reserved.
//

#include "ResourcePath.hpp"

#include <string>
#include <iostream>

#if defined(__APPLE__)
#include <SDL2/SDL_filesystem.h>
#else
#include "SDL_filesystem.h"
#endif

std::string ResourcePath() {
    if (gResourcePath.length() == 0) {
        char *path = SDL_GetPrefPath("Jack", "Wayward");
        gResourcePath = std::string(path) + "Resources\\";
        SDL_free(path);
	}

    return gResourcePath;
}
