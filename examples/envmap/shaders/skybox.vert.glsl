#include "base.vert.glsl"

void main() {
    gl_Position = (projection * view * vec4(aPosition, 1.0)).xyww;
    TexCoords3D = aPosition;
}
