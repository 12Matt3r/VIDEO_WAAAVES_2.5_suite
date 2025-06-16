#version 120
uniform sampler2DRect u_tex0;
uniform vec2 u_resolution;
uniform int u_sortMode;       // 0: Horizontal, 1: Vertical
uniform int u_sortCriteria;   // 0: Brightness, 1: Hue, 2:R, 3:G, 4:B, 5:Luminance
uniform float u_thresholdMin;
uniform float u_thresholdMax;
uniform bool u_sortAscending;
uniform bool u_smartThresholding;
uniform int u_sortWindowSize; // Max size of the local sort window (e.g., 3 to 15)

const int MAX_SORT_WINDOW = 15; // Must match or be >= max u_sortWindowSize in UI

float get_sort_value(vec4 pixelColor, int criteria) {
    if (criteria == 0) return (pixelColor.r + pixelColor.g + pixelColor.b) / 3.0; // Approx Brightness
    if (criteria == 5) return dot(pixelColor.rgb, vec3(0.299, 0.587, 0.114)); // Luminance
    if (criteria == 1) { // Hue (basic, 0-1 range)
        float maxC = max(pixelColor.r, max(pixelColor.g, pixelColor.b));
        float minC = min(pixelColor.r, min(pixelColor.g, pixelColor.b));
        float delta = maxC - minC;
        if (delta == 0.0) return 0.0; // Achromatic
        float r_ = pixelColor.r; float g_ = pixelColor.g; float b_ = pixelColor.b;
        float hue;
        if (maxC == r_) hue = (g_ - b_) / delta;
        else if (maxC == g_) hue = 2.0 + (b_ - r_) / delta;
        else hue = 4.0 + (r_ - g_) / delta;
        hue = fract(hue / 6.0); // Normalize to 0-1
        return hue;
    }
    if (criteria == 2) return pixelColor.r;
    if (criteria == 3) return pixelColor.g;
    if (criteria == 4) return pixelColor.b;
    return dot(pixelColor.rgb, vec3(0.299, 0.587, 0.114)); // Default to Luminance
}

void main() {
    vec2 current_coord = gl_FragCoord.xy;
    vec4 original_color = texture2DRect(u_tex0, current_coord);
    float current_sort_val = get_sort_value(original_color, u_sortCriteria);

    bool is_sortable = true;
    if (u_smartThresholding) { // "Smart" means current pixel must be within range to START a segment
        if (current_sort_val < u_thresholdMin || current_sort_val > u_thresholdMax) {
            is_sortable = false;
        }
    } else { // Simpler threshold: only consider sorting if current pixel meets min OR max (non-exclusive for segment start)
        // This mode is trickier; for now, let's assume it means if it passes *either* bound check, it might start a sort segment.
        // The actual segment breaking is handled in the loop.
        // For this initial shader, let's make non-smart equivalent to smart to avoid confusion.
        // A true non-smart mode would require finding start/end of segments more explicitly.
        if (current_sort_val < u_thresholdMin || current_sort_val > u_thresholdMax) {
            is_sortable = false;
        }
    }

    if (!is_sortable) {
        gl_FragColor = original_color;
        return;
    }

    // Collect pixels in the window
    vec4 pixel_window[MAX_SORT_WINDOW];
    float sort_values[MAX_SORT_WINDOW];
    int actual_window_size = 0;

    for (int i = 0; i < MAX_SORT_WINDOW; ++i) {
        if (i >= u_sortWindowSize) break;

        vec2 sample_coord = current_coord;
        if (u_sortMode == 0) { // Horizontal
            sample_coord.x += float(i);
        } else { // Vertical
            sample_coord.y += float(i);
        }

        if (sample_coord.x >= u_resolution.x || sample_coord.y >= u_resolution.y) {
            break;
        }

        vec4 s_color = texture2DRect(u_tex0, sample_coord);
        float s_val = get_sort_value(s_color, u_sortCriteria);

        // For both smart and non-smart (as currently simplified), the segment breaks if a pixel is outside range
        if (s_val < u_thresholdMin || s_val > u_thresholdMax) {
            break;
        }

        pixel_window[actual_window_size] = s_color; // Use actual_window_size as index
        sort_values[actual_window_size] = s_val;
        actual_window_size++;
    }

    if (actual_window_size <= 1) { // If window is 0 or 1, no sort needed or possible for the first pixel
        gl_FragColor = original_color;
        return;
    }

    // Simple Bubble Sort
    for (int i = 0; i < actual_window_size - 1; ++i) {
        for (int j = 0; j < actual_window_size - i - 1; ++j) {
            bool should_swap;
            if (u_sortAscending) {
                should_swap = sort_values[j] > sort_values[j+1];
            } else {
                should_swap = sort_values[j] < sort_values[j+1];
            }
            if (should_swap) {
                float temp_val = sort_values[j];
                sort_values[j] = sort_values[j+1];
                sort_values[j+1] = temp_val;

                vec4 temp_col = pixel_window[j];
                pixel_window[j] = pixel_window[j+1];
                pixel_window[j+1] = temp_col;
            }
        }
    }

    gl_FragColor = pixel_window[0];
}
