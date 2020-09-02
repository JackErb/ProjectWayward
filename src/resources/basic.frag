#version 400 core
out vec4 FragColor;

in GS_OUT {
	vec2 texCoord;
} fs_in;

uniform sampler2D texture1;

void main() {
    FragColor = texture(texture1, fs_in.texCoord);
	//if (FragColor.w < .1) FragColor = vec4(1,0,0,0);
} 
