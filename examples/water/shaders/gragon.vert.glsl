#version 460 core

layout(location=0) in vec3 aPosition;
layout(location=1) in vec3 aNormal;
layout(location=2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float scale;

uniform bool fv;
uniform bool fh;

out vec2 texCoords;
out vec3 FragPos;
out vec3 Normal;

void main() {
    mat4 mvp = projection * view * model;
    gl_Position = mvp * vec4(aPosition, 1.0);
    FragPos = vec3(model * vec4(aPosition, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;

    vec2 tex = (aTexCoords * 2.0 - 1.0) * scale;
    if (fv) tex.y *= -1.0;
    if (fh) tex.x *= -1.0;
    tex = tex / 2.0 + 0.5;
    texCoords = tex;
}
