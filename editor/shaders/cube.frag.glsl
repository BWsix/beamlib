#version 460
uniform vec3 lightColor;

in vec3 NormalDir;
in vec3 LightDir;
in vec3 FragPos;

out vec4 FragColor;

vec3 ambient = vec3(0.1, 0.1, 0.1);

void main() {
    vec3 viewDir = -FragPos;
    vec3 reflectDir = reflect(LightDir, NormalDir);

    float diffuseIntensity = max(0.0, dot(-LightDir, NormalDir));
    vec3 diffuse = 0.4 * vec3(diffuseIntensity);

    float specularIntensity = pow(max(0.0, dot(reflectDir, viewDir)), 128.0f);
    vec3 specular = 0.2 * vec3(specularIntensity);

    FragColor = vec4(lightColor * (ambient + diffuse + specular), 1.0);
}
