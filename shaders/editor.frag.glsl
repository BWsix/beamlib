#include "base.frag.glsl"

in flat uint FaceID;
in flat uint TexID;

uniform vec3 color;

void main() {
    vec3 lightDir = vec3(0.0, -1.0, 0.0);
    vec3 reflectDir = reflect(lightDir, Normal);

    float ambientIntensity = 0.3;
    vec3 ambient = ambientIntensity * color;
    // vec3 ambient = ambientIntensity * vec3(texture(material.texture_diffuse1, TexCoords));

    float diffuseIntensity = max(0.0, dot(Normal, -lightDir));
    vec3 diffuse = diffuseIntensity * color;
    // vec3 diffuse = diffuseIntensity * vec3(texture(material.texture_diffuse1, TexCoords));

    // float specularIntensity = pow(max(0.0, dot(reflectDir, normalize(-Position))), 64.0);
    // vec3 specular = specularIntensity * color;
    // vec3 specular = specularIntensity * vec3(texture(material.texture_specular1, TexCoords));

    // vec3 result = ambient + diffuse + specular;
    vec3 result = ambient + diffuse;
    FragColor = vec4(result, 1.0);
    // FragColor = vec4(vec3(float(FaceID) / 100000.0f), 1.0);
    // FragColor = vec4(0.5, 0.5, 0.5, 1.0);
}
