#version 330 core
#include "spiralMap.glsl"

layout (location = 0) in vec3 aPos;

uniform mat4 vp;
uniform mat4 model;

void main() {
    mat4 delta = mat4(1);
    delta[3].xz = 17.5 * spiralMap(gl_InstanceID + 1);

    mat4 m = delta * model;
    gl_Position = vp * m * vec4(aPos, 1.0);
}  
