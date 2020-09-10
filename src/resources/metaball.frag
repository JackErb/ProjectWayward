#version 400 core
out vec4 FragColor;

uniform int metaballsSize;
uniform vec3 metaballs[100];

uniform vec2 screenSize;
uniform vec2 cameraPos;
uniform float screenScale;

in VS_OUT {
    vec2 gamePosition;
} fs_in;

void main(){
    vec2 frag_pos = fs_in.gamePosition;
    float v = 0.0;
    for (int i = 0; i < metaballsSize; i++) {
        vec3 mb = metaballs[i];
        float dx = mb.x - frag_pos.x;
        float dy = mb.y - frag_pos.y;
        float r = mb.z;
        v += r*r/(dx*dx + dy*dy);
    }
    if (v > 1.0) {
        FragColor = vec4(0.4, 0.3, 0.7, 1.0);
    } else {
        FragColor = vec4(0.0, 0.0, 0.0, 0.0);
    }
}