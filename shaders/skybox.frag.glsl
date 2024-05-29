#include "base.frag.glsl"

layout(location=0) out vec4 Color;

const float exposure = 1.0;

void main() {
    vec3 color = vec3(texture(skybox, TexCoords3D));
    Color = vec4(color, 1.0);
}

// void main() {
//     vec3 color = vec3(texture(skybox, TexCoords3D));
//
//     const float gamma = 2.2;
//     // exposure tone mapping
//     vec3 mapped = vec3(1.0) - exp(-color.rgb * exposure);
//     mapped = pow(mapped, vec3(1.0 / gamma));
//     Color = vec4(mapped, 1.0);
// }
