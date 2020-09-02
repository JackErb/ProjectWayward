#include "resource_path.h"
#if defined(__APPLE__)
#include <SDL2/SDL_filesystem.h>
#endif

std::string resourcePath() {
    if (gResourcePath.length() == 0) {
      #if defined(__APPLE__)
        char *path  = SDL_GetBasePath();
        gResourcePath = std::string(path);
      #else
        char *path = SDL_GetPrefPath("Jack", "Wayward");
        gResourcePath = std::string(path) + "Resources\\";
      #endif
        SDL_free(path);
    }

    return gResourcePath;
}
