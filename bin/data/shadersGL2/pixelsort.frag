// pixelsort.frag
#version 120
uniform sampler2DRect u_tex0; // Input texture
uniform vec2 u_resolution;

uniform int u_sortMode; // 0: Horizontal, 1: Vertical
uniform int u_sortCriteria; // 0: Brightness, 1: Hue, ...
uniform float u_thresholdMin;
uniform float u_thresholdMax;
uniform bool u_sortAscending;
// uniform float u_effectMix; // Shader could do the mix with original (passed as another tex or by offsetting reads)

float get_sort_value(vec4 pixelColor, int criteria) {
    if (criteria == 0) return (pixelColor.r + pixelColor.g + pixelColor.b) / 3.0; // Approx Brightness
    if (criteria == 5) return dot(pixelColor.rgb, vec3(0.299, 0.587, 0.114)); // Luminance
    if (criteria == 1) { // Hue (very basic placeholder)
        float maxC = max(pixelColor.r, max(pixelColor.g, pixelColor.b));
        float minC = min(pixelColor.r, min(pixelColor.g, pixelColor.b));
        if (maxC == minC) return 0.0; // Achromatic
        float r_ = pixelColor.r; float g_ = pixelColor.g; float b_ = pixelColor.b;
        float hue;
        if (maxC == r_) hue = (g_ - b_) / (maxC - minC);
        else if (maxC == g_) hue = 2.0 + (b_ - r_) / (maxC - minC);
        else hue = 4.0 + (r_ - g_) / (maxC - minC);
        hue = hue * 60.0; if (hue < 0.0) hue = hue + 360.0;
        return hue / 360.0; // Normalize 0-1
    }
    if (criteria == 2) return pixelColor.r;
    if (criteria == 3) return pixelColor.g;
    if (criteria == 4) return pixelColor.b;
    return dot(pixelColor.rgb, vec3(0.299, 0.587, 0.114)); // Default to Luminance
}

void main() {
    vec2 st = gl_FragCoord.xy;
    vec4 original_color = texture2DRect(u_tex0, st);
    vec4 final_color = original_color;

    float sort_val = get_sort_value(original_color, u_sortCriteria);

    if (sort_val >= u_thresholdMin && sort_val <= u_thresholdMax) {
        // Simple "streaking" effect: if in threshold, sample from a neighbor based on sort direction
        // This is NOT sorting, but a common simple way to start a "pixel sort" shader.
        if (u_sortMode == 0) { // Horizontal streak
            float streak_source_x = u_sortAscending ? st.x - 1.0 : st.x + 1.0; // Check immediate neighbor
            // For a wider streak, use a larger offset or a loop, but be careful with performance.
            // streak_source_x = u_sortAscending ? st.x - 10.0 : st.x + 10.0;
            streak_source_x = clamp(streak_source_x, 0.0, u_resolution.x - 1.0);
            final_color = texture2DRect(u_tex0, vec2(streak_source_x, st.y));
        } else { // Vertical streak
            float streak_source_y = u_sortAscending ? st.y - 1.0 : st.y + 1.0; // Check immediate neighbor
            // streak_source_y = u_sortAscending ? st.y - 10.0 : st.y + 10.0;
            streak_source_y = clamp(streak_source_y, 0.0, u_resolution.y - 1.0);
            final_color = texture2DRect(u_tex0, vec2(st.x, streak_source_y));
        }
    }
    gl_FragColor = final_color;
    // The C++ side currently handles the mix. If shader were to do it:
    // gl_FragColor = mix(original_color, final_color, u_effectMix);
    // This would require u_effectMix uniform and original_color to be correctly sourced.
}
