#include "ShaderLoader.h"

#include "ResourceManager.h"
#include <iostream>

using std::string;
using std::cout;
using std::cerr;
using std::endl;

unsigned int loadShader(const string &file_name, GLenum shader_type) {
    int length;
    char *shaderSource = loadTextFile(file_name, &length);

    unsigned int vertexShader;
    vertexShader = glCreateShader(shader_type);
    glShaderSource(vertexShader, 1, &shaderSource, &length);
    glCompileShader(vertexShader);

    delete[] shaderSource;

    int success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        cerr << "ERROR::SHADER::COMPILATION_FAILED: " << file_name << endl;
        cerr << infoLog << endl;;
    } else {
        cout << "Shader: " << file_name << " succesfully compiled" << endl;
    }
    return vertexShader;
}

void linkProgram(unsigned int shaderProgram) {
    glLinkProgram(shaderProgram);

    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED" << endl;
        cout << infoLog << endl;
    } else {
        cout << "Shader succesfully compiled" << endl;
    }
}

unsigned int loadShaderProgram(const string &vert, const string &frag) {
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    
    unsigned int vertexShader = loadShader(vert, GL_VERTEX_SHADER);
    unsigned int fragmentShader = loadShader(frag, GL_FRAGMENT_SHADER);

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    linkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return shaderProgram;
}

unsigned int loadShaderProgram(const string &vert, const string &geom, const string &frag) {
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    
    unsigned int vertexShader = loadShader(vert, GL_VERTEX_SHADER);
    unsigned int geomShader = loadShader(geom, GL_GEOMETRY_SHADER);
    unsigned int fragmentShader = loadShader(frag, GL_FRAGMENT_SHADER);

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, geomShader);
    glAttachShader(shaderProgram, fragmentShader);
    linkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(geomShader);
    glDeleteShader(fragmentShader);
    
    return shaderProgram;
}
