#version 430 core
out vec4 FragColor;

in vec3 Position;
in vec3 Normal;
in vec2 TexCoords;
in vec3 TexCoords3D;

in vec4 LightSpacePosition;

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

float ShadowCalculation(sampler2D shadowMap, vec4 fragPosLightSpace) {
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow

    float bias = 0.01;
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    return shadow;
}  

