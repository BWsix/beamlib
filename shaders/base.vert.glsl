#version 430 core
layout(location=0) in vec3 aPosition;
layout(location=1) in vec3 aNormal;
layout(location=2) in vec2 aTexCoords;

out vec3 Position;
out vec3 Normal;
out vec2 TexCoords;
out vec3 TexCoords3D;

out vec4 LightSpacePosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
