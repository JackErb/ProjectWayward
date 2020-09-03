#include "Shader.h"

#include "resource_path.h"
#include <iostream>
#include <fstream>

using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::ifstream;

unsigned int loadShader(string file_name, GLenum shader_type) {
	string path = resourcePath() + file_name;
	ifstream file;
    file.exceptions(file.exceptions() | std::ios::failbit);
    
	try {
        file.open(path);
    } catch (std::ios_base::failure& e) {
        cerr << "Failed to open file: " << path << endl;
        cerr << e.what() << endl;
        return -1;
    }

	// Get length of file
	file.seekg(0, std::ios::end);
	int length = file.tellg();
	file.seekg(0, std::ios::beg);

	// Read the file
    char *shaderSource;
	shaderSource = new char[length];
	file.read(shaderSource, length);
	file.close();

	unsigned int vertexShader;
	vertexShader = glCreateShader(shader_type);
	glShaderSource(vertexShader, 1, &shaderSource, &length);
	glCompileShader(vertexShader);

	int success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    	cout << "ERROR::SHADER::COMPILATION_FAILED: " << file_name << endl;
		cout << infoLog << endl;;
	} else {
		cout << "Shader: " << file_name << " succesfully compiled" << endl;
	}
	return vertexShader;
}


unsigned int loadShaderProgram() {
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	
	unsigned int vertexShader = loadShader("basic.vert", GL_VERTEX_SHADER);
	unsigned int geomShader = loadShader("basic.geom", GL_GEOMETRY_SHADER);
	unsigned int fragmentShader = loadShader("basic.frag", GL_FRAGMENT_SHADER);

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, geomShader);
	glAttachShader(shaderProgram, fragmentShader);
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
    
    glDeleteShader(vertexShader);
	glDeleteShader(geomShader);
    glDeleteShader(fragmentShader);
    
    return shaderProgram;
}
