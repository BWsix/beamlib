#version 430 core

struct Lighting {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Lighting lighting;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform samplerCube skybox;

layout(location=0) out vec4 FragColor;

void main() {
    vec3 lightDir = normalize(lightPos - FragPos);
    // vec3 lightDir = vec3(0.0, 1.0, 0.0);
    vec3 normal = normalize(Normal);

    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lighting.ambient;

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lighting.diffuse;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float specularStrength = 0.5;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), lighting.shininess);
    vec3 specular = specularStrength * spec * lighting.specular;  

    vec3 result = (ambient + diffuse + specular);

    vec3 R = reflect(-viewDir, normal);
    vec3 reflect = texture(skybox, R).rgb;
    if (lighting.shininess > 360.0) {
        result += 0.5 * reflect;
    }

    FragColor = vec4(result, 1.0);
}
