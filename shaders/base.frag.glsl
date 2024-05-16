#version 430 core
out vec4 FragColor;

in vec3 Position;
in vec3 Normal;
in vec2 TexCoords;
in vec3 TexCoords3D;

struct Lighting {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    sampler2D texture_diffuse3;
    sampler2D texture_specular1;
    sampler2D texture_specular2;
    sampler2D texture_specular3;
};

uniform Material material;
uniform Lighting lighting;
uniform samplerCube skybox;

uniform vec3 lightPos;
uniform vec3 viewPos;
