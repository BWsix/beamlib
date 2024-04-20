#include "base.frag.glsl"

void main() {
    vec3 lightDir = vec3(0.0, -1.0, 0.0);
    vec3 reflectDir = reflect(lightDir, Normal);

    float ambientIntensity = 0.3;
    vec3 ambient = ambientIntensity * vec3(1.0);

    float diffuseIntensity = max(0.0, dot(Normal, -lightDir));
    vec3 diffuse = diffuseIntensity * vec3(1.0);

    float specularIntensity = pow(max(0.0, dot(reflectDir, normalize(-Position))), 64.0);
    vec3 specular = specularIntensity * vec3(1.0);

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
