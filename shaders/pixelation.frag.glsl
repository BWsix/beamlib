#version 430 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform vec2 screenSize;
uniform float pixelSize;

void main() { 
    float dx = pixelSize / screenSize.x;
    float dy = pixelSize / screenSize.y;
    vec2 coord = vec2(dx*floor(TexCoords.x/dx) + dx / 2.0,
                      dy*floor(TexCoords.y/dy) + dy / 2.0);
    FragColor = texture(screenTexture, coord);
}
