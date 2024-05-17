#include "base.vert.glsl"

uniform mat4 prevModel;
uniform mat4 prevViewProjection;
uniform mat4 lightVP;

out vec4 ClipSpacePos;
out vec4 PrevClipSpacePos;

void main() {
    mat4 mvp = projection * view * model;
    gl_Position = mvp * vec4(aPosition, 1.0);
    Position = vec3(model * vec4(aPosition, 1.0));
    LightSpacePosition = lightVP * model * vec4(aPosition, 1.0);
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aTexCoords;
} 
