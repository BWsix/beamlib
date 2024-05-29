vec3 quantizeColor(vec3 color) {
    // Define the number of levels for each color channel
    const float levels = 32.0; // 2^8 = 256
    
    // Quantize each color channel
    vec3 quantizedColor = floor(color * levels) / levels;
    
    return quantizedColor;
}

vec3 tooner(vec3 color, float intensity) {
    // vec3 result = quantizeColor(color);
    vec3 result = color;
    if (intensity > 0.95) {
        result *= vec3(1.0);
    } else if (intensity > 0.9){ 
        result *= vec3(0.9);
    } else if (intensity > 0.6) {
        result *= vec3(0.6);
    } else {
        result *= vec3(0.3);
    }
    return result;
}
