#include "base.frag.glsl"

layout(location=0) out vec4 color;

uniform sampler2D depthTex;

void main() {
    float shadow = ShadowCalculation(depthTex, LightSpacePosition);
    vec3 result = vec3(1 - shadow);
    float gamma = 2.2;
    color = vec4(pow(result, vec3(gamma)), 1.0);
}
