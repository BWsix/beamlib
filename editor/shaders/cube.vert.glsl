#version 460
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aNormal;
layout(location=2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

vec3 light = vec3(0, -1, 0);

out vec3 NormalDir;
out vec3 LightDir;
out vec3 FragPos;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    NormalDir = normalize(mat3(transpose(inverse(view * model))) * aNormal);
    FragPos = vec3(view * model * vec4(aPos, 1.0f));
    LightDir = normalize(mat3(transpose(inverse(view))) * light);
}
