#version 430 core

uniform vec3 lightColor;

layout(location=0) out vec4 FragColor;

void main() {
    FragColor = vec4(lightColor, 1.0);
}
