#version 430 core

in vec4 LightSpacePosition;

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

    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}  


layout (location = 0) out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform samplerCube skybox;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform vec3 waterColor;

uniform sampler2D depthTex;
uniform sampler2D reflectionTex;
uniform sampler2D refractionTex;

void main() {
    vec2 uv = gl_FragCoord.xy / textureSize(reflectionTex, 0);
    float dist = distance(FragPos, viewPos);
    vec3 lightDir = normalize(lightPos - FragPos);

    // smooth the normal with distance to avoid disturbing high frequency noise
    vec3 normal = normalize(mix(Normal, vec3(0.0, 1.0, 0.0), 0.8 * min(1.0, sqrt(dist * 0.01) * 1.1)));
    uv.x += 0.02 * normal.x;
    uv.y -= 0.02 * normal.z;

    vec3 viewDir = normalize(viewPos - FragPos);
    // vec3 skyColor = vec3(texture(skybox, reflect(-viewDir, normal)));

    vec3 reflectDir = reflect(-lightDir, normal);
    float specular = pow(max(dot(viewDir, reflectDir), 0.0), 720) * 210;

    vec3 reflection = vec3(texture(reflectionTex, vec2(uv.x, 1 - uv.y)));
    vec3 refraction = vec3(texture(refractionTex, uv));

    float fresnel = 0.04 + (1.0 - 0.04) * pow(1.0 - max(0.0, dot(normal, viewDir)), 5.0);
    // float fresnel = dot(viewDir, vec3(0, 1, 0));
    // vec3 result = specular * fresnel + mix(reflection * fresnel, refraction * (1 - fresnel), 0.0);
    vec3 result = mix(refraction * (1 - fresnel), (reflection + specular) * fresnel, clamp(3.5 * fresnel, 0.0, 1.0));
    result = mix(result, waterColor, 0.05);

    float shadow = ShadowCalculation(depthTex, LightSpacePosition);
    if (shadow == 1.0) {
        result *= 0.3;
    }
    FragColor = vec4(result, 1.0);
    // FragColor = vec4(vec3(fresnel), 1.0);
}
