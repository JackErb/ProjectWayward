#define STB_IMAGE_IMPLEMENTATION

#include "TextureLoader.h"
#include <glad/glad.h>
#include <stb_image.h>
#include "resource_path.h"

#include <iostream>

using std::string;
using std::cout;
using std::cerr;
using std::endl;

unsigned int loadTexture(string file_name) {
	unsigned int texture_id;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, num_channels;
	string file_path = resourcePath() + file_name;
	stbi_set_flip_vertically_on_load(true); 
	unsigned char *data = stbi_load(file_path.c_str(), &width, &height, &num_channels, STBI_rgb_alpha);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA, width, height, GL_TRUE);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		cerr << "Failed to load texture" << endl;
	}
	stbi_image_free(data);
	
	return texture_id;
}
