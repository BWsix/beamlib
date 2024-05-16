#version 430 core

uniform vec3 lightColor;

layout(location=0) out vec4 FragColor;

void main() {
    vec2 p = gl_PointCoord * 2 - 1;
    if (length(p) > 1) {
        discard;
    }
    FragColor = vec4(vec3(100), 1.0);
}
