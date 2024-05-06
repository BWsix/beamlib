#include "base.vert.glsl"

layout(location=3) in uint aFaceID;
layout(location=4) in uint aTexID;

out flat uint FaceID;
out flat uint TexID;

void main() {
    mat4 mv = view * model;
    gl_Position = projection * mv * vec4(aPosition, 1.0);
    Position = vec3(mv * vec4(aPosition, 1.0));
    Normal = normalize(mat3(transpose(inverse(mv))) * aNormal);
    TexCoords = aTexCoords;
    FaceID = aFaceID;
    TexID = aTexID;
}
