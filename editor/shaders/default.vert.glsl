#include "hi.vert.glsl"

void main() {
  mat4 mv = view * model;
  gl_Position = projection * mv * vec4(aPosition, 1.0);
  Position = vec3(model * vec4(aPosition, 1.0));
  Normal = mat3(transpose(inverse(model))) * aNormal;
  TexCoords = aTexCoords;
}
