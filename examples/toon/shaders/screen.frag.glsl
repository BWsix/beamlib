#version 430 core

in vec2 TexCoord;

layout(location=0) out vec4 FragColor;

uniform sampler2D screenTex;
uniform float threshold;

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
    vec4 n[9];
    make_kernel(n, screenTex, TexCoord, 1.2);

    if (length(sobel(n).rgb) > threshold) {
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    } else {
        FragColor = texture(screenTex, TexCoord);
    }

    // sharpen
    // float kernel[9] = float[](
    //     -1, -1, -1,
    //     -1,  9, -1,
    //     -1, -1, -1
    // );

    // blur
    // float kernel[9] = float[](
    //     1.0 / 16, 2.0 / 16, 1.0 / 16,
    //     2.0 / 16, 4.0 / 16, 2.0 / 16,
    //     1.0 / 16, 2.0 / 16, 1.0 / 16  
    // );

    // edge detection
    // float kernel[9] = float[](
    //     1,  1, 1,
    //     1, -8, 1,
    //     1,  1, 1
    // );

    
    // vec3 sampleTex[9];
    // for(int i = 0; i < 9; i++) {
    //     sampleTex[i] = vec3(texture(screenTex, TexCoord.st + offsets[i]));
    // }
    // vec3 col = vec3(0.0);
    // for(int i = 0; i < 9; i++) {
    //     col += sampleTex[i] * kernel[i];
    // }
    // 
    // FragColor = vec4(col, 1.0);
}  
