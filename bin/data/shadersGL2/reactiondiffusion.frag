#version 120
uniform sampler2DRect u_prevState; // Previous state: R=U, G=V
uniform vec2 u_resolution;
uniform float u_feedRate;
uniform float u_killRate;
uniform float u_diffuseRateU;
uniform float u_diffuseRateV;
uniform float u_timeStep; // dt for simulation step

// Kernel for Laplacian (9-tap)
const mat3 kernel = mat3(
    0.05, 0.2, 0.05,
    0.2, -1.0, 0.2,
    0.05, 0.2, 0.05
);

void main() {
    vec2 st = gl_FragCoord.xy;
    vec2 onePixel = vec2(1.0, 1.0) / u_resolution;

    // Sample current U and V
    vec2 uv = texture2DRect(u_prevState, st).rg;
    float u = uv.x;
    float v = uv.y;

    // Calculate Laplacian for U and V
    float laplacianU = 0.0;
    laplacianU += texture2DRect(u_prevState, st + vec2(-onePixel.x, -onePixel.y)).r * kernel[0][0];
    laplacianU += texture2DRect(u_prevState, st + vec2( 0.0,       -onePixel.y)).r * kernel[0][1];
    laplacianU += texture2DRect(u_prevState, st + vec2( onePixel.x, -onePixel.y)).r * kernel[0][2];
    laplacianU += texture2DRect(u_prevState, st + vec2(-onePixel.x,  0.0)).r       * kernel[1][0];
    laplacianU += u                                                              * kernel[1][1]; // Center pixel
    laplacianU += texture2DRect(u_prevState, st + vec2( onePixel.x,  0.0)).r       * kernel[1][2];
    laplacianU += texture2DRect(u_prevState, st + vec2(-onePixel.x,  onePixel.y)).r * kernel[2][0];
    laplacianU += texture2DRect(u_prevState, st + vec2( 0.0,        onePixel.y)).r * kernel[2][1];
    laplacianU += texture2DRect(u_prevState, st + vec2( onePixel.x,  onePixel.y)).r * kernel[2][2];

    float laplacianV = 0.0;
    laplacianV += texture2DRect(u_prevState, st + vec2(-onePixel.x, -onePixel.y)).g * kernel[0][0];
    laplacianV += texture2DRect(u_prevState, st + vec2( 0.0,       -onePixel.y)).g * kernel[0][1];
    laplacianV += texture2DRect(u_prevState, st + vec2( onePixel.x, -onePixel.y)).g * kernel[0][2];
    laplacianV += texture2DRect(u_prevState, st + vec2(-onePixel.x,  0.0)).g       * kernel[1][0];
    laplacianV += v                                                              * kernel[1][1]; // Center pixel
    laplacianV += texture2DRect(u_prevState, st + vec2( onePixel.x,  0.0)).g       * kernel[1][2];
    laplacianV += texture2DRect(u_prevState, st + vec2(-onePixel.x,  onePixel.y)).g * kernel[2][0];
    laplacianV += texture2DRect(u_prevState, st + vec2( 0.0,        onePixel.y)).g * kernel[2][1];
    laplacianV += texture2DRect(u_prevState, st + vec2( onePixel.x,  onePixel.y)).g * kernel[2][2];

    float uvv = u * v * v;
    float newU = u + (u_diffuseRateU * laplacianU - uvv + u_feedRate * (1.0 - u)) * u_timeStep;
    float newV = v + (u_diffuseRateV * laplacianV + uvv - (u_feedRate + u_killRate) * v) * u_timeStep;

    gl_FragColor = vec4(clamp(newU, 0.0, 1.0), clamp(newV, 0.0, 1.0), 0.0, 1.0);
}
