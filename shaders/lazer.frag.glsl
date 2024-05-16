#include "base.frag.glsl"

layout(location=1) out vec4 brightColor;

void main() {
    int intensity = 100;
    brightColor = vec4(vec3(intensity), 0.0);
}
