#ifndef TextureLoader_h
#define TextureLoader_h

#include <string>
#include <vector>

unsigned int loadTexture(std::string file_name);
unsigned int loadTextures(std::vector<std::string> files);

#endif  /* TextureLoader_h */
