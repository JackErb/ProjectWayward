#version 400 core
out vec4 FragColor;

in GS_OUT {
	vec2 texCoord;
} fs_in;
flat in int fs_texture;

uniform sampler2D texture_sampler;

void main() {
    FragColor = texture(texture_sampler, fs_in.texCoord);
} 
