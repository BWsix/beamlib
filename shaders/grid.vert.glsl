#include "base.vert.glsl"

uniform int width;

void main() {
    int side_length = width * 2 + 1;
    int x_offset = gl_InstanceID / side_length - width;
    int z_offset = gl_InstanceID % side_length - width;

    gl_Position = projection * view * model * vec4(aPosition.x + x_offset, 0.0f, aPosition.z + z_offset, 1.0f);
}
