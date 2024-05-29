#version 430 core

layout(location=0) in vec3 aPosition;

out vec4 LightSpacePosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 lightVP;

uniform int halfSideLength;
uniform float delta;

uniform float time;
uniform int iterations;
#include "wave.glsl"

out vec3 Normal;
out vec3 FragPos;

vec2 getXY() {
    int sideLength = halfSideLength * 2 + 1;
    int x_offset = gl_InstanceID % sideLength - halfSideLength;
    int z_offset = gl_InstanceID / sideLength - halfSideLength;
    return vec2(aPosition.x + delta * x_offset, aPosition.z + delta * z_offset);
}

void main() {
    vec2 xy = getXY();
    float h = getwaves(xy);

    vec3 pos = vec3(xy.x, h, xy.y);
    Normal = getnormal(xy, h);

    // pos = vec3(xy.x, 0, xy.y);
    // Normal = vec3(0, 1, 0);

    LightSpacePosition = lightVP * model * vec4(pos, 1.0);

    gl_Position = projection * view * model * vec4(pos, 1.0f);
    FragPos = vec3(model * vec4(pos, 1.0f));
}
