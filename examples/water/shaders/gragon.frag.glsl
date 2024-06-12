#version 460 core

layout(location=0) out vec4 fragColor;

in vec3 FragPos;
uniform bool reflectionMode;

uniform sampler2D diffuse;
in vec2 texCoords;

void main() {
    if (reflectionMode && FragPos.y < 0){
        discard;
    }

    fragColor = texture(diffuse, texCoords);
}
