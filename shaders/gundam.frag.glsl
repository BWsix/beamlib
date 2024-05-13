#include "base.frag.glsl"

layout(location=0) out vec4 color;
layout(location=2) out vec4 motion;

in vec4 ClipSpacePos;
in vec4 PrevClipSpacePos;

void main() {
    vec3 lightDir = vec3(0.0, -1.0, 0.0);
    vec3 reflectDir = reflect(lightDir, Normal);

    float ambientIntensity = 0.3;
    vec3 ambient = ambientIntensity * lighting.diffuse;
    // vec3 ambient = ambientIntensity * vec3(texture(material.texture_diffuse1, TexCoords));

    float diffuseIntensity = max(0.0, dot(Normal, -lightDir));
    vec3 diffuse = diffuseIntensity * lighting.diffuse;
    // vec3 diffuse = diffuseIntensity * vec3(texture(material.texture_diffuse1, TexCoords));

    float specularIntensity = pow(max(0.0, dot(reflectDir, normalize(-Position))), 64);
    vec3 specular = specularIntensity * lighting.diffuse;
    // vec3 specular = specularIntensity * vec3(texture(material.texture_specular1, TexCoords));

    vec3 result = ambient + diffuse + specular;
    color = vec4(result, 1.0);

    vec3 NDCPos = (ClipSpacePos / ClipSpacePos.w).xyz;
    vec3 PrevNDCPos = (PrevClipSpacePos / PrevClipSpacePos.w).xyz;
    motion = vec4((NDCPos - PrevNDCPos).xy, -101, 0.0);
}
