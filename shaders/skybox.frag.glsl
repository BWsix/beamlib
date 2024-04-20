#include "base.frag.glsl"

void main() {
    FragColor = texture(skybox, TexCoords3D);
    // FragColor = vec4(1.0);
}
