#version 430 core
layout(location=0) in vec4 aPosition;
layout(location=1) in vec3 aNormal;
layout(location=2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

smooth out vec3 FragPos;
smooth out vec3 Normal;
smooth out vec2 TexCoord;

void main() {
    FragPos = vec3(model * vec4(aPosition.xyz, 1.0));

    gl_Position = projection * view * model * vec4(aPosition.xyz, 1.0);

    // todo: do somethine with depth?
    gl_PointSize = aPosition.w;
}
