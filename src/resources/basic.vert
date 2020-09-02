#version 400 core
layout (location= 0) in vec2 aPos;
layout (location= 1) in vec2 aSize;

uniform vec2 screenSize;
uniform vec2 cameraPos;
uniform float screenScale;

out VS_OUT {
	vec2 size;
} vs_out;

void main() {
	vec2 pos = (aPos + cameraPos) / screenSize * screenScale;
    gl_Position = vec4(pos, 0.0, 1.0);
	vs_out.size = aSize / screenSize * screenScale;
}
