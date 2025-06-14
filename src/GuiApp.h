#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include <string>
#include <vector>

// Constants for array sizes, useful for iteration and initialization
const int MAX_INPUT_CHANNELS_GUI = 2;
const int MAX_FB_CHANNELS_GUI = 4;

class GuiApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    void drawOutputWindow(ofTexture* output_texture);

    ofxImGui::Gui imgui_instance;

    struct LFOSettings {
        float amp {0.0f};
        float theta {0.0f};
    };

    struct ChannelSettings {
        int select {1};
        float bright {1.0f};
        float hue {1.0f};
        float saturation {1.0f};
        bool hue_alt_invert_toggle {false};
        bool saturation_alt_invert_toggle {false};
        bool bright_alt_invert_toggle {false};
        bool saturation_wrap {false};
        bool bright_wrap {false};
        float bright_powmap {1.0f};
        float hue_powmap {1.0f};
        float saturation_powmap {1.0f};
        LFOSettings hue_lfo;
        LFOSettings saturation_lfo;
        LFOSettings bright_lfo;
        float mix_value {0.0f};
        float key_value {0.0f};
    };
    ChannelSettings channel_settings[MAX_INPUT_CHANNELS_GUI];

    struct FeedbackSettings {
        float key_value {0.0f}; float key_threshold {0.0f}; int delay_amount {0}; float mix {0.0f};
        bool hflip_switch {false}; bool vflip_switch {false}; int toroid_switch {0};
        float hue {10.0f}; float saturation {10.0f}; float bright {10.0f};
        bool hue_invert {false}; bool saturation_invert {false}; bool bright_invert {false};
        float huex_mod {10.0f}; float huex_offset {0.0f}; float huex_lfo_val {0.0f};
        float x_displace {0.0f}; float y_displace {0.0f}; float z_displace {100.0f}; float rotate {0.0f};
        float x_displace_range {40.0f}; float y_displace_range {40.0f}; float z_displace_range {10.0f}; float rotate_range {OF_PI};

        int texmod_select {1}; bool tex_mod_enabled {false};
        float tex_key_value {0.0f}; // tex_fb_key_value
        float tex_key_threshold {0.0f}; // tex_fb_key_threshold
        float tex_mix {0.0f}; // tex_fb_mix
        float tex_hue {0.0f}; // tex_fb_hue
        float tex_saturation {0.0f}; // tex_fb_saturation
        float tex_bright {0.0f}; // tex_fb_bright
        float tex_huex_mod {0.0f}; // tex_fb_huex_mod
        float tex_huex_offset {0.0f}; // tex_fb_huex_offset
        float tex_huex_lfo_val {0.0f}; // tex_fb_huex_lfo_val
        float tex_x_displace {0.0f}; // tex_fb_x_displace
        float tex_y_displace {0.0f}; // tex_fb_y_displace
        float tex_z_displace {0.0f}; // tex_fb_z_displace
        float tex_rotate {0.0f}; // tex_fb_rotate

        LFOSettings lumakeyvalue_lfo; LFOSettings lumakeythreshold_lfo; LFOSettings x_lfo; LFOSettings y_lfo; LFOSettings z_lfo; LFOSettings rotate_lfo;
        LFOSettings mix_lfo; LFOSettings delay_lfo; LFOSettings hue_lfo; LFOSettings saturation_lfo; LFOSettings bright_lfo;
        LFOSettings huexmod_lfo; LFOSettings huexoffset_lfo; LFOSettings huexlfo_val_lfo; // Matches huex_lfo_val

        bool pixel_switch {false}; int pixel_scale_x {64}; int pixel_scale_y {64}; float pixel_mix {1.0f}; float pixel_brightscale {0.0f};
        int pixel_texmod_select {0}; bool pixel_texmod_enabled {false};
        int texmod_pixel_scale_x {0}; int texmod_pixel_scale_y {0}; float texmod_pixel_mix {0.0f}; float texmod_pixel_brightscale {0.0f};
        LFOSettings pixel_scale_x_lfo; LFOSettings pixel_scale_y_lfo; LFOSettings pixel_mix_lfo; LFOSettings pixel_brightscale_lfo;
    };
    FeedbackSettings feedback_settings[MAX_FB_CHANNELS_GUI];

    struct GlobalSettings {
        int global_texmod_select {0}; bool texmod_enabled {false};
        bool control_reset_trigger {false}; bool framebuffer_clear_trigger {false};
        float blur_amount {0.0f}; float blur_radius {1.0f}; float sharpen_amount {0.0f}; float sharpen_radius {1.0f}; float sharpen_boost {0.0f};
        float cam1_scale {1.0f}; float cam2_scale {1.0f}; float ndi_scale {0.0f};
        bool cam1_hflip_switch {false}; bool cam1_vflip_switch {false}; bool cam2_hflip_switch {false}; bool cam2_vflip_switch {false};
        float x_skew {0.0f}; float y_skew {0.0f};
        bool tetrahedron_switch {false}; bool hypercube_switch {false}; float hypercube_theta_rate {0.01f}; float hypercube_phi_rate {0.01f};
        bool ndiOutput_enabled {false};

        float texmod_blur_amount {0.0f}; float texmod_blur_radius {1.0f}; float texmod_sharpen_amount {0.0f}; float texmod_sharpen_radius {1.0f}; float texmod_sharpen_boost {0.0f};

        LFOSettings blur_amount_lfo; LFOSettings blur_radius_lfo; LFOSettings sharpen_amount_lfo; LFOSettings sharpen_radius_lfo; LFOSettings sharpen_boost_lfo;

        bool cam1_pixel_switch {false}; int cam1_pixel_scale_x {64}; int cam1_pixel_scale_y {64}; float cam1_pixel_mix {1.0f}; float cam1_pixel_brightscale {0.0f};
        LFOSettings cam1_pixel_scale_x_lfo; LFOSettings cam1_pixel_scale_y_lfo; LFOSettings cam1_pixel_mix_lfo; LFOSettings cam1_pixel_brightscale_lfo;

        bool cam2_pixel_switch {false}; int cam2_pixel_scale_x {64}; int cam2_pixel_scale_y {64}; float cam2_pixel_mix {1.0f}; float cam2_pixel_brightscale {0.0f};
        LFOSettings cam2_pixel_scale_x_lfo; LFOSettings cam2_pixel_scale_y_lfo; LFOSettings cam2_pixel_mix_lfo; LFOSettings cam2_pixel_brightscale_lfo;

        bool ndi_pixel_switch {false}; int ndi_pixel_scale_x {64}; int ndi_pixel_scale_y {64}; float ndi_pixel_mix {1.0f}; float ndi_pixel_brightscale {0.0f};
        LFOSettings ndi_pixel_scale_x_lfo; LFOSettings ndi_pixel_scale_y_lfo; LFOSettings ndi_pixel_mix_lfo; LFOSettings ndi_pixel_brightscale_lfo;
    };
    GlobalSettings global_settings;

    struct ParticleFeedbackSettings {{
        bool enableParticleFeedback {{false}};
        int inputSource {{0}}; // 0: MainOutput, 1: Cam1, 2: Cam2, 3: NDI, 4: VideoFile, 5: WindowCapture
        float spawnThreshold {{0.5f}};
        int maxParticles {{1000}};
        float particleInitialLife {{2.0f}}; // in seconds
        float particleInitialSpeed {{50.0f}}; // pixels per second
        float particleDrag {{0.05f}};
        float particleSize {{2.0f}};
        bool enableVelocityFromBrightness {{true}};
        bool inheritColorFromSpawn {{true}};
        ofColor particleBaseColor {{ofColor::white}};
        float noiseForceAmount {{0.0f}};
        float noiseFieldScale {{0.01f}};
        float noiseTimeSpeed {{0.1f}};
        float feedbackMix {{0.1f}}; // How much of particle FBO is blended back
        // Potential future additions:
        // bool drawParticleTrails {{false}};
        // int trailLength {{10}};
        // float particleRotation {{0.0f}};
        // float particleRotationSpeed {{0.0f}};
    }};
    ParticleFeedbackSettings particle_feedback_settings;

    struct SlitScanSettings {
        bool enableSlitScan {{false}};
        int inputSource {{0}}; // 0: MainOutput, 1: Cam1, 2: Cam2, 3: NDI, 4: VideoFile, 5: WindowCapture
        int slitDirection {{0}}; // 0: Vertical Slit (scans X, accumulates along X), 1: Horizontal Slit (scans Y, accumulates along Y)
        float slitPosition {{0.5}}; // Normalized (0.0 to 1.0)
        int slitThickness {{2}}; // In pixels
        float accumulationSpeed {{1.0}}; // Pixels per frame, can be negative
        bool wrapAccumulation {{true}};
        int delayFrames {{0}};
        float outputMix {{1.0}}; // Blend factor of slit-scan output to main
        int blendMode {{0}}; // 0: Alpha Blend, 1: Add, 2: Screen (example blend modes)
    };
    SlitScanSettings slit_scan_settings;

    struct NoiseGeneratorSettings {
        bool enableNoise {{false}};
        int noiseType {{0}}; // 0: Perlin/ofNoise, 1: Simplex (future)
        float noiseScale {{0.02f}};
        float noiseTime {{0.0f}};
        float noiseSpeed {{0.05f}};
        bool noiseAnimateTime {{true}};
        int noiseOctaves {{4}};
        float noisePersistence {{0.5f}};
        bool noiseColorEnable {{false}};
        ofColor noiseColor1 {{ofColor::black}};
        ofColor noiseColor2 {{ofColor::white}};
        float noiseRangeMin {{0.0f}}; // ofNoise default is 0-1
        float noiseRangeMax {{1.0f}};
        bool noiseApplyContrast {{false}};
        float noiseContrast {{1.0f}};
        float noiseBrightness {{0.0f}}; // Additive brightness
    };
    NoiseGeneratorSettings noise_generator_settings;

    struct PixelSortSettings {
        bool enablePixelSort {{false}};
        int inputSource {{0}}; // 0: Main Output (current fbo_draw), 1: Cam1, etc.
        int sortMode {{0}};    // 0: Horizontal Lines, 1: Vertical Columns
        int sortCriteria {{0}};// 0: Brightness, 1: Hue, 2: Red, 3: Green, 4: Blue, 5: Luminance
        float thresholdMin {{0.1f}};
        float thresholdMax {{0.9f}};
        bool sortAscending {{true}};
        // bool smartThresholding {{true}}; // For sorting segments between min/max
        float effectMix {{1.0f}}; // Blend between original and sorted (0=original, 1=sorted)
        // int delayFrames {{0}}; // Optional: Delay source before sorting
    };
    PixelSortSettings pixel_sort_settings;

    std::string loadedVideoPath {"No video loaded"};
    bool videoPlayingState {false}; bool videoLoopingState {true};
    bool loadVideoTrigger {false}; bool playVideoTrigger {false}; bool pauseVideoTrigger {false}; bool stopVideoTrigger {false}; bool loopVideoToggleTrigger {false};

    std::vector<std::string> availableWindowTitles;
    int selectedWindowIndex {-1};
    bool isWindowCurrentlyCapturing {false};
    std::string capturedWindowTitleDisplay {"None"};
    bool listWindowsTrigger {false}; bool startCaptureTrigger {false}; bool stopCaptureTrigger {false};

    // Updated draw function declarations
    void drawChannelControls(int channel_idx, float lfom);
    void drawFeedbackControls(int fb_idx, float lfom);
    void drawGlobalControls(float lfom);
    void drawPixelateControls(const std::string& panel_name_id, float lfom);
    void drawVideoControls();
    void drawWindowCaptureControls();
    void drawMoreEffectsControls();
};
