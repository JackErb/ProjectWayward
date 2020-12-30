#include <graphics.h>
#include <resources.h>
#include <iostream>
#include <glad.h>

using std::string;
using std::cerr;
using std::endl;

unsigned int graphics::LoadTexture(const string &file) {
  unsigned int texture_id;
  int width, height;
  unsigned char *data = resources::ReadPng(file, &width, &height);
  if (data) {
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    cerr << "Texture " << texture_id << " initialized" << endl;
  } else {
    cerr << "Failed to load texture" << endl;
  }
  resources::FreePng(data);
  
  return texture_id;
}
