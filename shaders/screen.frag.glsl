#version 430 core

in vec2 TexCoords;

layout(location=0) out vec4 Color;

uniform bool blur;
uniform sampler2D screenTex;
uniform sampler2D motion;

uniform bool pixelation;
uniform vec2 screenSize;
uniform float pixelSize;

void main() {
    Color = vec4(texture2D(screenTex, TexCoords).rgb, 1.0);

    if (blur) {
        if (texture2D(motion, TexCoords).b > -100) {
            Color = vec4(texture2D(screenTex, TexCoords).rgb, 1.0);
        } else {
            vec2 TexCoord = TexCoords;
            vec2 MotionVector = texture2D(motion, TexCoord).rg;
            Color = texture2D(screenTex, TexCoord) * 0.4;
            TexCoord -= MotionVector;
            Color += texture2D(screenTex, TexCoord) * 0.3;
            TexCoord -= MotionVector;
            Color += texture2D(screenTex, TexCoord) * 0.2;
            TexCoord -= MotionVector;
            Color += texture2D(screenTex, TexCoord) * 0.1;
        }
    }

    if (pixelation) {
        float dx = pixelSize / screenSize.x;
        float dy = pixelSize / screenSize.y;
        vec2 coord = vec2(dx*floor(TexCoords.x/dx) + dx / 2.0, dy*floor(TexCoords.y/dy) + dy / 2.0);
        Color = texture2D(screenTex, coord);
    }
}
