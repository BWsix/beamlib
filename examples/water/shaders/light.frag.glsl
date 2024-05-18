#version 430 core

uniform vec3 lightColor;

layout(location=0) out vec4 FragColor;

in vec3 FragPos;

uniform bool reflectionMode;

void main() {
    if (reflectionMode && FragPos.y < 0){
        discard;
    }

    FragColor = vec4(lightColor, 1.0);
}
