#version 460
layout(location=0) in vec3 aPos;
layout(location=1) in float aBrightness;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out float brightness;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    brightness = aBrightness;
}
