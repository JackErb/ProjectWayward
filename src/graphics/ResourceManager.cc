#include "ResourceManager.h"
#include "resource_path.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <string>
#include <iostream>
#include <vector>
#include <fstream>

using std::vector;
using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::ifstream;

unsigned char *loadPng(string file_name, int *width, int *height) {
    int num_channels;
    string file_path = resourcePath() + file_name;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(file_path.c_str(), width, height, &num_channels, STBI_rgb_alpha);
    cout << file_name << " dim: " << *width << ", " << *height << endl;
    return data;
}

void freePng(unsigned char *data) {
    stbi_image_free(data);
}

char *loadTextFile(string file_name, int *length) {
    string path = resourcePath() + file_name; 
    ifstream file; 
    file.exceptions(file.exceptions() | std::ios::failbit); 
     
    try {
        file.open(path); 
    } catch (std::ios_base::failure& e) { 
        cerr << "Failed to open file: " << path << endl; 
        cerr << e.what() << endl; 
        return nullptr; 
    } 
 
    // Get length of file 
    file.seekg(0, std::ios::end); 
    *length = file.tellg(); 
    file.seekg(0, std::ios::beg); 
 
    // Read the file 
    char *shaderSource; 
    char *source = new char[*length]; 
    file.read(source, *length); 
    file.close();
    return source;
}
