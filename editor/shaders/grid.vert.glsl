#version 450
layout(location=0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform int width;

void main() {
    int side_length = width * 2 + 1;
    int x_offset = gl_InstanceID / side_length - width;
    int z_offset = gl_InstanceID % side_length - width;

    gl_Position = projection * view * model * vec4(aPos.x + x_offset, 0.0f, aPos.z + z_offset, 1.0f);
}
