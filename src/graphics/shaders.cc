#include <graphics.h>
#include <glad.h>
#include <iostream>
#include <resources.h>

using std::string;
using std::cout;
using std::cerr;
using std::endl;

unsigned int load_shader(const string &file_name, GLenum shader_type) {
  int length;
  char *source = resources::ReadTextFile(file_name, &length);

  unsigned int shader_prog;
  shader_prog = glCreateShader(shader_type);
  glShaderSource(shader_prog, 1, &source, &length);
  glCompileShader(shader_prog);

  delete[] source;

  // Check compilation status
  // @Todo: Change this to use logging interface rather than printing to cout/cerr.
  int success;
  glGetShaderiv(shader_prog, GL_COMPILE_STATUS, &success);
  if (!success) {
      char infoLog[512];
      glGetShaderInfoLog(shader_prog, 512, NULL, infoLog);
      cerr << "ERROR::SHADER::COMPILATION_FAILED: " << file_name << endl;
      cerr << infoLog << endl;;
  } else {
      cout << "Shader: " << file_name << " succesfully compiled" << endl;
  }
  return shader_prog;
}

void link_program(unsigned int shader_program) {
  glLinkProgram(shader_program);

  // Check compilation status
  // @Todo: Change this to use logging interface rather than printing to cout/cerr.
  int success;
  glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
  if (!success) {
      char infoLog[512];
      glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
      cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED" << endl;
      cout << infoLog << endl;
  } else {
      cout << "Shader " << shader_program << " succesfully linked & compiled" << endl;
  }
}

unsigned int graphics::LoadShaderProgram(const string &vert, const string &geom, const string &frag) {
  unsigned int shader_program;
  shader_program = glCreateProgram();

  unsigned int vertex_shader = load_shader(vert, GL_VERTEX_SHADER);
  unsigned int geom_shader = load_shader(geom, GL_GEOMETRY_SHADER);
  unsigned int frag_shader = load_shader(frag, GL_FRAGMENT_SHADER);

  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, geom_shader);
  glAttachShader(shader_program, frag_shader);
  link_program(shader_program);
  glDeleteShader(vertex_shader);
  glDeleteShader(geom_shader);
  glDeleteShader(frag_shader);

  return shader_program;
}
