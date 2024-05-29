#version 430 core

in vec2 TexCoords;

layout(location=0) out vec4 Color;

uniform sampler2D screenTex;
uniform sampler2D motion;
uniform sampler2D depth;
uniform sampler2D depthTex;
uniform bool motionBlur;
uniform mat4 invViewProjection;
uniform mat4 prevViewProjection;

uniform bool pixelation;
uniform vec2 screenSize;
uniform float pixelSize;

uniform sampler2D blur;
uniform float exposure;

uniform bool toon;
uniform float toonThreshold;

void make_kernel(inout vec4 n[9], sampler2D tex, vec2 coord, float weight) {
    ivec2 sz = textureSize(tex, 0);
	float w = weight / sz.x;
	float h = weight / sz.y;

	n[0] = texture(tex, coord + vec2( -w, -h));
	n[1] = texture(tex, coord + vec2(0.0, -h));
	n[2] = texture(tex, coord + vec2(  w, -h));
	n[3] = texture(tex, coord + vec2( -w, 0.0));
	n[4] = texture(tex, coord);
	n[5] = texture(tex, coord + vec2(  w, 0.0));
	n[6] = texture(tex, coord + vec2( -w, h));
	n[7] = texture(tex, coord + vec2(0.0, h));
	n[8] = texture(tex, coord + vec2(  w, h));
}

vec4 sobel(inout vec4 n[9]) {
	vec4 sobel_edge_h = n[2] + (2.0*n[5]) + n[8] - (n[0] + (2.0*n[3]) + n[6]);
  	vec4 sobel_edge_v = n[0] + (2.0*n[1]) + n[2] - (n[6] + (2.0*n[7]) + n[8]);
	vec4 sobel = sqrt((sobel_edge_h * sobel_edge_h) + (sobel_edge_v * sobel_edge_v));
    return sobel;
}


void main() {
    vec4 color = vec4(texture(screenTex, TexCoords).rgb, 1.0);

    if (toon) {
        vec4 n[9];
        make_kernel(n, screenTex, TexCoords, 1.2);

        if (length(sobel(n).rgb) > toonThreshold) {
            color = vec4(0.0, 0.0, 0.0, 1.0);
        }
    }

    if (motionBlur) {
        vec2 MotionVector;

        float zOverW = texture(depth, TexCoords).r;
        vec4 currentPos = vec4(TexCoords.x * 2.0 - 1.0, TexCoords.y * 2.0 - 1.0, zOverW, 1.0);
        vec4 D = invViewProjection * currentPos;
        vec4 worldPos = D / D.w; 
        vec4 previousPos = prevViewProjection * worldPos;
        previousPos = previousPos / vec4(previousPos.w);
        MotionVector = vec2(currentPos.xy - previousPos.xy) / 10;

        if (texture(motion, TexCoords).b > 100) {
            MotionVector = texture(motion, TexCoords).rg / 3;
        }

        int factor = 10;
        vec4 c = texture(screenTex, TexCoords);
        vec2 texCoord = TexCoords;
        for(int i = 1; i < factor; i++) {
            texCoord -= MotionVector;
            vec4 currentColor = texture(screenTex, texCoord);   
            c += currentColor; 
        }
        color = vec4(vec3(c / vec4(factor)), 1.0);
    }

    if (pixelation) {
        float dx = pixelSize / screenSize.x;
        float dy = pixelSize / screenSize.y;
        vec2 coord = vec2(dx*floor(TexCoords.x/dx) + dx / 2.0, dy*floor(TexCoords.y/dy) + dy / 2.0);
        color = texture(screenTex, coord);
    }

    // blur
    vec3 bloomColor = texture(blur, TexCoords).rgb;
    color.rgb += bloomColor;

    const float gamma = 2.2;
    // exposure tone mapping
    vec3 mapped = vec3(1.0) - exp(-color.rgb * exposure);
    mapped = pow(mapped, vec3(1.0 / gamma));
    Color = vec4(mapped, 1.0);
}
