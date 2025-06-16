#version 120
uniform sampler2DRect u_rdStateTex; // R=U, G=V
uniform int u_colorMode; // 0: U as Gray, 1: V as Gray, 2: U/V to Colors
uniform vec4 u_colorU;
uniform vec4 u_colorV;
uniform bool u_smoothDisplay; // If true, apply a simple blur
uniform vec2 u_resolution; // Needed for blur if textureSize() not available

void main() {
    vec2 st = gl_FragCoord.xy;
    vec2 uv_val = texture2DRect(u_rdStateTex, st).rg;
    float u = uv_val.x;
    float v = uv_val.y;

    if (u_smoothDisplay) {
        // Simple 3x3 box blur for smoothing
        vec2 onePixel = vec2(1.0, 1.0) / u_resolution;

        vec2 sum_uv = vec2(0.0);
        for (int x = -1; x <= 1; x++) {
            for (int y = -1; y <= 1; y++) {
                sum_uv += texture2DRect(u_rdStateTex, st + vec2(x,y) * onePixel).rg;
            }
        }
        uv_val = sum_uv / 9.0;
        u = uv_val.x;
        v = uv_val.y;
    }

    vec3 final_color;
    if (u_colorMode == 0) { // U as Grayscale
        final_color = vec3(u);
    } else if (u_colorMode == 1) { // V as Grayscale
        final_color = vec3(v);
    } else { // U/V to Colors
        final_color = mix(u_colorU.rgb * u, u_colorV.rgb * v, v);
    }
    gl_FragColor = vec4(clamp(final_color, 0.0, 1.0), 1.0);
}
