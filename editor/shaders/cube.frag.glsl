#version 460
uniform vec3 lightColor;

in vec3 NormalDir;
in vec3 LightDir;
in vec3 FragPos;

out vec4 FragColor;

vec3 ambient = vec3(0.3);

void main() {
    vec3 viewDir = -FragPos;
    vec3 reflectDir = reflect(LightDir, NormalDir);

    float diffuseIntensity = max(0.0, dot(-LightDir, NormalDir));
    vec3 diffuse = 0.6 * vec3(diffuseIntensity);

    FragColor = vec4((ambient + diffuse) * lightColor , 1.0);
}
