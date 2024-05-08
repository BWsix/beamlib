#version 430 core

smooth in vec3 FragPos;
smooth in vec3 Normal;
smooth in vec2 TexCoord;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform vec3 objectColor;

layout(location=0) out vec4 FragColor;

void main() {
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 normal = normalize(Normal);

    float intensity = max(dot(normal, lightDir), 0.0);

	vec3 result;
	if (intensity > 0.95) {
		result = objectColor * vec3(1.0);
    } else if (intensity > 0.7){ 
		result = objectColor * vec3(0.9);
    } else if (intensity > 0.5) {
		result = objectColor * vec3(0.6);
    } else {
		result = objectColor * vec3(0.3);
    }

    FragColor = vec4(lightColor * result, 1.0);
}
