#ifndef Shader_h
#define Shader_h

#include <string>
#include <glad/glad.h>

unsigned int loadShader(std::string file_name, GLenum type);
unsigned int loadShaderProgram();

#endif  /* Shader_h */
