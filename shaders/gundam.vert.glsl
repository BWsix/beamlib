#include "base.vert.glsl"
#include "spiralMap.glsl"

uniform mat4 prevModel;
uniform mat4 prevViewProjection;
uniform mat4 lightVP;

out vec4 ClipSpacePos;
out vec4 PrevClipSpacePos;

void main() {
    mat4 delta = mat4(1);
    delta[3].xz = 17.5 * spiralMap(gl_InstanceID + 1);

    mat4 m = delta * model;
    mat4 mvp = projection * view * m;
    gl_Position = mvp * vec4(aPosition, 1.0);
    Position = vec3(m * vec4(aPosition, 1.0));
    LightSpacePosition = lightVP * m * vec4(aPosition, 1.0);
    Normal = mat3(transpose(inverse(m))) * aNormal;
    TexCoords = aTexCoords;

    ClipSpacePos = mvp * vec4(aPosition, 1.0);
    PrevClipSpacePos = prevViewProjection * delta * prevModel * vec4(aPosition, 1.0);
}
