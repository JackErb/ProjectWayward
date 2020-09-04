#version 400 core
out vec4 FragColor;

in GS_OUT {
	vec2 texCoord;
} fs_in;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
    FragColor = texture(texture2, fs_in.texCoord);
} 
