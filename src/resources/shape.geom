#version 400 core
layout (points) in;
layout (triangle_strip, max_vertices=4) out;

in VS_OUT {
	vec2 size;
} gs_in[];

void main() {
	vec4 pos = gl_in[0].gl_Position;
	vec2 spriteSize = vec2(gs_in[0].size.x / 2.0, gs_in[0].size.y / 2.0);

    gl_Position = pos + vec4(-spriteSize.x, -spriteSize.y, 0.0, 0.0);
    EmitVertex();

	gl_Position = pos + vec4(-spriteSize.x, spriteSize.y, 0.0, 0.0);
	EmitVertex();

	gl_Position = pos + vec4(spriteSize.x, -spriteSize.y, 0.0, 0.0);
	EmitVertex();

	gl_Position = pos + vec4(spriteSize.x, spriteSize.y, 0.0, 0.0);
	EmitVertex();

	EndPrimitive();
}  
