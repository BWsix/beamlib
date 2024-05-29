#version 430 core

in vec2 TexCoords;

layout(location=0) out vec4 Color;

uniform sampler2D screenTex;

void main() {
    vec4 color = vec4(texture(screenTex, TexCoords).rgb, 1.0);
    Color = color;
}
