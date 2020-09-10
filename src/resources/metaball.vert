#version 400 core
layout (location= 0) in vec2 position;

uniform vec2 screenSize;
uniform vec2 cameraPos;
uniform float screenScale;

out VS_OUT {
    vec2 gamePosition;
} vs_out;

void main() {
    gl_Position = vec4(position, 0.0, 1.0);
    vs_out.gamePosition = position / screenScale * screenSize - cameraPos;
}
