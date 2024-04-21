#include "base.frag.glsl"

void main() {
    FragColor = texture(material.texture_diffuse1, TexCoords);
}
