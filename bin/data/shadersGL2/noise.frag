// noise.frag
#version 120 // Or your target GLSL version for oF GL2

uniform vec2 u_resolution; // Screen/FBO resolution
uniform float u_time;       // Time for animation
uniform float u_scale;
uniform int u_octaves;
uniform float u_persistence;

uniform bool u_colorize;
uniform vec4 u_color1;
uniform vec4 u_color2;

uniform float u_rangeMin;
uniform float u_rangeMax;
uniform bool u_applyContrast;
uniform float u_contrast;
uniform float u_brightness;


// Basic 2D random function (can be replaced with a better hash)
float random(vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898,78.233))) * 43758.5453123);
}

// Basic Perlin-like noise (value noise, not gradient noise like ofNoise)
// For true Perlin/Simplex, you'd need a more complex implementation or built-in GLSL noise if available
float simple_noise(vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);
    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));
    vec2 u = f * f * (3.0 - 2.0 * f); // Smoothstep
    return mix(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.y * u.x;
}

// Fractal Brownian Motion (FBM) using simple_noise
float fbm(vec2 st, int octaves, float persistence) {
    float total = 0.0;
    float frequency = 1.0;
    float amplitude = 1.0;
    float maxValue = 0.0;  // Used for normalizing result to 0.0 - 1.0
    for(int i = 0; i < octaves; i++) {
        total += simple_noise(st * frequency) * amplitude;
        maxValue += amplitude;
        amplitude *= persistence;
        frequency *= 2.0;
    }
    if (maxValue > 0.0) return total / maxValue; // Normalize
    return total;
}


void main() {
    vec2 st = gl_FragCoord.xy / u_resolution.xy;
    // Adjust st by u_time for animation if needed, or pass u_time into noise function's z component
    vec2 noise_coord = st * u_scale; // Use u_scale here

    // Using a 3D noise where time is the z-component
    // This requires a 3D noise function. The simple_noise above is 2D.
    // For now, let's just use u_time to offset coordinates for a simpler animation.
    noise_coord.x += u_time * 0.1; // Simple animation by shifting coordinates

    // float n = simple_noise(noise_coord); // Single octave
    float n = fbm(noise_coord, u_octaves, u_persistence); // FBM

    // Remap noise from its typical range (e.g., 0-1 from fbm) to desired output range
    // If u_rangeMin/Max define the *input* range of raw noise, then:
    // n = (n - u_rangeMin) / (u_rangeMax - u_rangeMin); // Normalize to 0-1
    // For now, assume fbm output is already 0-1 or simple_noise is 0-1

    // Apply contrast and brightness (after 0-1 mapping)
    if (u_applyContrast) {
        n = (n - 0.5) * u_contrast + 0.5;
    }
    n += u_brightness;
    n = clamp(n, 0.0, 1.0);

    vec3 color;
    if (u_colorize) {
        color = mix(u_color1.rgb, u_color2.rgb, n);
    } else {
        color = vec3(n);
    }
    gl_FragColor = vec4(color, 1.0);
}
