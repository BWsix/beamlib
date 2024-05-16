#include "base.frag.glsl"

void main() {
    vec3 lightDir = normalize(lightPos - Position);
    vec3 normal = normalize(Normal);
    vec3 reflectDir = reflect(lightDir, normal);

    float ambientIntensity = 0.3;
    vec3 ambient = ambientIntensity * vec3(1.0);

    float diffuseIntensity = max(0.0, dot(normal, -lightDir));
    vec3 diffuse = diffuseIntensity * vec3(1.0);

    float specularIntensity = pow(max(0.0, dot(reflectDir, normalize(-Position))), 64.0);
    vec3 specular = specularIntensity * vec3(1.0);

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
