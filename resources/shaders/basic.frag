#version 400 core
out vec4 FragColor;

in GS_OUT {
	vec2 texCoord;
} fs_in;

uniform sampler2D textureSampler;

void main() {
  FragColor = texture(textureSampler, fs_in.texCoord);
}
