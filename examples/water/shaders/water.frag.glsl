#version 430 core

layout (location = 0) out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform samplerCube skybox;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform vec3 waterColor;

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

    vec3 reflection = vec3(texture2D(reflectionTex, vec2(uv.x, 1 - uv.y)));
    vec3 refraction = vec3(texture2D(refractionTex, uv));

    float fresnel = 0.04 + (1.0 - 0.04) * pow(1.0 - max(0.0, dot(normal, viewDir)), 5.0);
    // float fresnel = dot(viewDir, vec3(0, 1, 0));
    // vec3 result = specular * fresnel + mix(reflection * fresnel, refraction * (1 - fresnel), 0.0);
    vec3 result = mix(refraction * (1 - fresnel), (reflection + specular) * fresnel, clamp(3.5 * fresnel, 0.0, 1.0));
    result = mix(result, waterColor, 0.05);

    FragColor = vec4(result, 1.0);
    // FragColor = vec4(vec3(fresnel), 1.0);
}
