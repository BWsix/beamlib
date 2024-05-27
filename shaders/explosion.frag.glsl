#version 430 core

layout(location=0) out vec4 FragColor;
layout(location=1) out vec4 brightColor;

// flat in float iTime;
uniform float time;

float snoise(vec3 uv, float res) {
    const vec3 s = vec3(1e0, 1e2, 1e3);

    uv *= res;

    vec3 uv0 = floor(mod(uv, res))*s;
    vec3 uv1 = floor(mod(uv+vec3(1.), res))*s;

    vec3 f = fract(uv); f = f*f*(3.0-2.0*f);

    vec4 v = vec4(uv0.x+uv0.y+uv0.z, uv1.x+uv0.y+uv0.z,
                  uv0.x+uv1.y+uv0.z, uv1.x+uv1.y+uv0.z);

    vec4 r = fract(sin(v*1e-1)*1e3);
    float r0 = mix(mix(r.x, r.y, f.x), mix(r.z, r.w, f.x), f.y);

    r = fract(sin((v + uv1.z - uv0.z)*1e-1)*1e3);
    float r1 = mix(mix(r.x, r.y, f.x), mix(r.z, r.w, f.x), f.y);

    return mix(r0, r1, f.z)*2.-1.;
}

void main() {
    float iTime = time + gl_PrimitiveID;

    vec2 p = -.5 + gl_PointCoord;
    p *= 0.85;

    float color = 3.0 - (3.*length(2.*p));

    vec3 coord = vec3(atan(p.x,p.y)/6.2832+.5, length(p)*.4, .5);

    for(int i = 1; i <= 7; i++) {
        float power = pow(2.0, float(i));
        color += (1.5 / power) * snoise(coord + vec3(0.,-iTime*.1, iTime*.02), power*16.);
    }

    vec3 finalColor = vec3(color, pow(max(color,0.),2.)*0.4, pow(max(color,0.),3.)*0.15);
    if (length(vec3(finalColor)) < 1.1 || length(gl_PointCoord * 2 - 1) > 1.25) {
        discard;
    }
    FragColor = vec4(finalColor, length(finalColor) * length(gl_PointCoord * 2 - 1) * 0.2);
    brightColor = vec4(finalColor, 1.0);
}
