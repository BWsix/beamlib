#version 430 core
layout(location=0) in vec3 aPosition;
layout(location=1) in vec3 aNormal;
layout(location=2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Position;
out vec3 Normal;
out vec2 TexCoords;

void main() {
  mat4 mv = view * model;
  gl_Position = projection * mv * vec4(aPosition, 1.0);
  Position = vec3(model * vec4(aPosition, 1.0));
  Normal = mat3(transpose(inverse(model))) * aNormal;
  TexCoords = aTexCoords;
}
