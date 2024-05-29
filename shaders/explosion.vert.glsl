#version 430 core
layout(location=0) in vec4 aPosition;
layout(location=1) in vec3 aNormal;
layout(location=2) in vec2 aTexCoord;

uniform int sz;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

flat out float iTime;
out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

void main() {
    FragPos = vec3(model * vec4(aPosition.xyz, 1.0));

    gl_Position = projection * view * model * vec4(aPosition.xyz, 1.0);
    iTime = time + gl_InstanceID * 100;

    // todo: do somethine with depth?
    gl_PointSize = aPosition.w;
}
