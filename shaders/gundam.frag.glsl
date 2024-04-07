#include "base.frag.glsl"

vec3 lightDir = vec3(0.0, -1.0, 0.0);

void main() {
    // vec3 reflectDir = reflect(lightDir, normal);
    //
    float ambientIntensity = 0.3;
    // vec3 ambient = ambientIntensity * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 ambient = ambientIntensity * lighting.diffuse;
    //
    float diffuseIntensity = max(0.0, dot(Normal, -lightDir));
    // vec3 diffuse = diffuseIntensity * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 diffuse = diffuseIntensity * lighting.diffuse;
    //
    // float specularIntensity = pow(max(0.0, dot(reflectDir, normalize(-Position))), 16.0);
    // vec3 specular = specularIntensity * vec3(texture(material.texture_specular1, TexCoords));
    //
    // vec3 result = ambient + diffuse + specular;
    // FragColor = vec4(result, 1.0);
    // FragColor = texture(material.texture_diffuse1, TexCoords);

    // FragColor = vec4(texture(skybox, reflect(Position, normal)).rgb, 1.0);
    // FragColor = vec4(reflect(Position, normal), 1.0);

    // FragColor = vec4(texture(material.texture_diffuse1, TexCoords).rgb, 1.0);
    FragColor = vec4(ambient + diffuse, 1.0);
}
