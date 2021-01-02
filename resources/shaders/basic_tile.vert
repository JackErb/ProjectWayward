#version 400 core
layout (location= 0) in vec2 aPos;
layout (location= 1) in vec2 aSize;
layout (location= 2) in int aTexture;
layout (location= 3) in int aDir;

uniform vec2 screenSize;
uniform vec2 cameraPos;
uniform float screenScale;

out VS_OUT {
	vec2 size;
  int texture;
  int dir;
} vs_out;

void main() {
	vec2 pos = (aPos + cameraPos) / screenSize * screenScale;
  gl_Position = vec4(pos, 0.0, 1.0);

	vec2 pixel_offset = 5.0 / screenSize;
	vs_out.size = aSize / screenSize * screenScale + pixel_offset;
  vs_out.texture = aTexture;
  vs_out.dir = aDir;
}
