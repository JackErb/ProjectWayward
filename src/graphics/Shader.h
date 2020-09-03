#ifndef Shader_h
#define Shader_h

#include <string>
#include <glad/glad.h>

unsigned int loadShader(const std::string &file_name, GLenum type);
unsigned int loadShaderProgram(const std::string &vert, 
                               const std::string &geom, 
                               const std::string &frag);

#endif  /* Shader_h */
