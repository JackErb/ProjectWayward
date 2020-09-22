#version 400 core
layout (location= 0) in vec2 position;

uniform vec2 size;

void main() {
	vec2 pos = position / size;
    gl_Position = vec4(pos, 0.0, 1.0);
}
