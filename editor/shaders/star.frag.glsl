#version 450
uniform vec3 lightColor;

in float brightness;
out vec4 FragColor;

void main() {
    FragColor = vec4(brightness * lightColor, 1.0);
}
