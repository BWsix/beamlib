#include "base.frag.glsl"

void main() {
    vec3 lightDir = vec3(0.0, -1.0, 0.0);
    vec3 reflectDir = reflect(lightDir, Normal);

    float ambientIntensity = 0.3;
    vec3 ambient = ambientIntensity * lighting.diffuse;
    // vec3 ambient = ambientIntensity * vec3(texture(material.texture_diffuse1, TexCoords));

    float diffuseIntensity = max(0.0, dot(Normal, -lightDir));
    vec3 diffuse = diffuseIntensity * lighting.diffuse;
    // vec3 diffuse = diffuseIntensity * vec3(texture(material.texture_diffuse1, TexCoords));

    float specularIntensity = pow(max(0.0, dot(reflectDir, normalize(-Position))), 64.0);
    vec3 specular = specularIntensity * lighting.diffuse;
    // vec3 specular = specularIntensity * vec3(texture(material.texture_specular1, TexCoords));

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
