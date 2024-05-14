#version 430 core

in vec2 TexCoords;

layout(location=0) out vec4 Color;

uniform bool blur;
uniform sampler2D screenTex;
uniform sampler2D motion;
uniform sampler2D depth;
uniform mat4 invViewProjection;
uniform mat4 prevViewProjection;

uniform bool pixelation;
uniform vec2 screenSize;
uniform float pixelSize;

void main() {
    Color = vec4(texture2D(screenTex, TexCoords).rgb, 1.0);

    if (blur) {
        vec2 MotionVector;

        float zOverW = texture2D(depth, TexCoords).r;
        vec4 currentPos = vec4(TexCoords.x * 2.0 - 1.0, TexCoords.y * 2.0 - 1.0, zOverW, 1.0);
        vec4 D = invViewProjection * currentPos;
        vec4 worldPos = D / D.w; 
        vec4 previousPos = prevViewProjection * worldPos;
        previousPos = previousPos / vec4(previousPos.w);
        MotionVector = vec2(currentPos.xy - previousPos.xy) / 10;

        if (texture2D(motion, TexCoords).b > 100) {
            MotionVector = texture2D(motion, TexCoords).rg / 2;
        }

        int factor = 10;
        vec4 color = texture2D(screenTex, TexCoords);
        vec2 texCoord = TexCoords;
        for(int i = 1; i < factor; i++) {
            texCoord -= MotionVector;
            vec4 currentColor = texture2D(screenTex, texCoord);   
            color += currentColor; 
        }
        Color = vec4(vec3(color / vec4(factor)), 1.0);
    }

    if (pixelation) {
        float dx = pixelSize / screenSize.x;
        float dy = pixelSize / screenSize.y;
        vec2 coord = vec2(dx*floor(TexCoords.x/dx) + dx / 2.0, dy*floor(TexCoords.y/dy) + dy / 2.0);
        Color = texture2D(screenTex, coord);
    }
}
