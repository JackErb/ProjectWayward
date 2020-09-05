#version 400 core
out vec4 FragColor;

in GS_OUT {
	vec2 texCoord;
} fs_in;
flat in int fs_texture;

uniform sampler2DArray textures;

void main() {
    FragColor = texture(textures, vec3(fs_in.texCoord, fs_texture));
} 
