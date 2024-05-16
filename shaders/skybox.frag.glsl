#include "base.frag.glsl"

layout(location=0) out vec4 color;

void main() {
    color = texture(skybox, TexCoords3D);
}
