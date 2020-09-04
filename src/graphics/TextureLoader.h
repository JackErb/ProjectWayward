#ifndef TextureLoader_h
#define TextureLoader_h

#include <string>
#include <vector>

namespace WaywardGL {

unsigned int loadTexture(std::string file_name);

unsigned int loadTextures(std::vector<std::string> files);

}

#endif  /* TextureLoader_h */
