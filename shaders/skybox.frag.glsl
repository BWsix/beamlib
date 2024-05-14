#include "base.frag.glsl"

void main() {
    FragColor = texture(skybox, TexCoords3D);
}
