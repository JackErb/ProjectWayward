#include <resources.h>
#include <iostream>
#include <fstream>
#include <SDL.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using std::string;
using std::ifstream;
using std::cout;
using std::cerr;
using std::endl;

string gResourcePath;

const string& ResourcePath() {
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

char *resources::ReadTextFile(const string &file_name, int *length) {
  string path = ResourcePath() + file_name; 
  ifstream file; 
  file.exceptions(file.exceptions() | std::ios::failbit); 
   
  try {
    file.open(path); 
  } catch (std::ios_base::failure& e) { 
    // @Todo: Switch to logging
    cerr << "Failed to open file: " << path << endl; 
    cerr << e.what() << endl; 
    return nullptr; 
  } 
 
  // Get length of file 
  file.seekg(0, std::ios::end); 
  *length = file.tellg(); 
  file.seekg(0, std::ios::beg); 
 
  // Read the file 
  char *source = new char[*length];
  file.read(source, *length); 
  file.close();
  return source;
}

unsigned char *resources::ReadPng(const string &file_name, int *width, int *height) {
  int num_channels;
  string path = ResourcePath() + file_name;
  
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load(path.c_str(), width, height, &num_channels, STBI_rgb_alpha);
  return data;
}

void resources::FreePng(unsigned char *data) {
  stbi_image_free(data);
}
